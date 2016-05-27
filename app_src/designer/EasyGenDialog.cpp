/*
 *   Copyright (C) 2016 Anton Haubner <anton.haubner@outlook.de>
 *
 *   This file is part of the PiGPIOSequence Designer program.
 *
 *   PiGPIOSequence Designer is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU Affero General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   PiGPIOSequence Designer is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Affero General Public License for more details.
 *
 *   You should have received a copy of the GNU Affero General Public License
 *   along with PiGPIOSequence Designer.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "EasyGenDialog.hpp"
#include "ui_EasyGenDialog.h"

#include <QVBoxLayout>
#include <QMessageBox>
#include <QShortcut>

#include <stdexcept>
#include <tuple>
#include <array>
#include <random>

#include <PiGPIOSequence/Extensions/Widgets/PinSelector.hpp>

using PinNum = PiGPIOSequence::Utils::PinNumber;

using namespace std;

EasyGenDialog::EasyGenDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::EasyGenDialog),
	m_Success(false)
{
	ui->setupUi(this);

	//Setup
	auto SetupLayout = [](QWidget * parent) -> QLayout *
	{
		auto Layout = new QVBoxLayout(parent);

		Layout->setMargin(0);

		parent->setLayout(Layout);

		return Layout;
	};

	auto addVoltCheck = [](QWidget * parent, QLayout * layout, unsigned int volts, PinNum pin) -> PinEnableInfo
	{
		auto Check = new QCheckBox(parent);
		Check->setText(QString::number(volts) + " Volt");

		layout->addWidget(Check);

		return PinEnableInfo(pin, volts, Check);
	};

	auto setupAxis = [this, &SetupLayout, &addVoltCheck](QWidget * parent, std::vector<PinNum> && pins, std::vector<unsigned int> && volts) -> AxisPinInfo
	{
		//Info
		if (pins.size() != volts.size())
		{
			throw runtime_error("Not the same number of pins as volts");
		}

		auto Layout = SetupLayout(parent);

		AxisPinInfo info;

		for (decltype(pins.size()) i = 0; i < pins.size(); i++)
		{
			info.push_back(addVoltCheck(parent, Layout, volts[i], pins[i]));
		}

        return info;
	};

	//X
	m_XInfo = setupAxis(ui->XVoltsFrame,

	{
						-1,
						18,
						23,
						24,
						25,
						12
	},

	{
						0,
						2,
						4,
						6,
						8,
						10
	});

	//Y
	m_YInfo = setupAxis(ui->YVoltsFrame,

	{
						-1,
						5,
						6,
						13,
						19,
						26
	},

	{
						0,
						2,
						4,
						6,
						8,
						10
	});

	//Z1
	m_Z1Info = setupAxis(ui->Z1VoltsFrame,

	{
						-1,
						17,
						27,
						22
	},

	{
						0,
						4,
						8,
						12
	});

	//Z2
	m_Z2Info = setupAxis(ui->Z2VoltsFrame,

	{
						-1,
						16,
						20,
						21
	},

	{
						0,
						4,
						8,
						12
	});

	//Listenshortcuts
	auto addShortcut = [this](QListWidget * parent)
	{
		auto DelShortcut = new QShortcut(QKeySequence(Qt::Key_Delete), parent, nullptr, nullptr, Qt::WidgetShortcut);
		connect(DelShortcut, SIGNAL(activated()), this, SLOT(on_TimeSubButton_clicked()));
	};

	addShortcut(ui->XDelays);
	addShortcut(ui->YDelays);
	addShortcut(ui->Z1Delays);
	addShortcut(ui->Z2Delays);

	auto AddShortcut = new QShortcut(QKeySequence(Qt::Key_Return), ui->TimeInputSpin, nullptr, nullptr, Qt::WidgetShortcut);
	connect(AddShortcut, SIGNAL(activated()), this, SLOT(on_TimeAddButton_clicked()));
}

EasyGenDialog::~EasyGenDialog()
{
	delete ui;
}

SequenceGenerator::UniqueSequencePtr EasyGenDialog::generate()
{
    exec();

	if (m_Success)
    {
        auto RPtr = std::make_unique<PiGPIOSequence::Sequence>();

		random_device rd;
		mt19937 engine(rd());

		using PinVector = std::vector<PiGPIOSequence::Utils::PinNumber>;

		auto collectPins = [](AxisPinInfo & container) -> PinVector
		{
			PinVector Target;

			for (auto & Info : container)
			{
				if (std::get<2>(Info)->isChecked())
				{
					Target.emplace_back(std::get<0>(Info));
				}
			}

            return Target;
		};

		using PinCollection_t = array<PinVector, 4>;
		PinCollection_t PinCollection
        {{
			collectPins(m_XInfo),
			collectPins(m_YInfo),
			collectPins(m_Z1Info),
			collectPins(m_Z2Info)
        }};

		using AxisCollection_t = array<AxisDelayTabs, 4>;
		AxisCollection_t AxisCollection
        {{
			AxisDelayTabs::X,
			AxisDelayTabs::Y,
			AxisDelayTabs::Z1,
			AxisDelayTabs::Z2
        }};

		bool inverted = true;

		//Alle gewählten GPIOs von Beginn an AN
		for (auto & Pins : PinCollection)
		{
			for (auto & GPIO : Pins)
			{
				if (GPIO != -1)
				{
                    RPtr->add({GPIO, inverted, 0});
				}
			}
		}

		PiGPIOSequence::Utils::Milliseconds CurrentDelay = 0;

		auto MaxElements = ui->MaxElemsSpin->value();
			if (MaxElements == 0)
			{
				MaxElements = -1;
			}

        auto MaxDelay = static_cast<PiGPIOSequence::Utils::Milliseconds>(ui->MaxDelaySpin->value());
			constexpr auto InvalidDelay = std::numeric_limits<PiGPIOSequence::Utils::Milliseconds>::max();

			if (MaxDelay == 0)
			{
				MaxDelay = InvalidDelay;
			}

		uniform_int_distribution<PinCollection_t::size_type> PinVecDist {0, PinCollection.size() - 1};

		while (MaxElements > 0 ||
			   (MaxElements == -1 && MaxDelay != InvalidDelay))
		{
			auto rand_PinVec = PinVecDist(engine);

			auto & PinVec = PinCollection[rand_PinVec];
			auto AxisTab = AxisCollection[rand_PinVec];
			auto & Delays = m_DelaysMap[AxisTab];

			if (Delays.empty() && !PinVec.empty())
			{
				QMessageBox::warning(this, "Problem", "Für jede Achse, für die Elemente erstellt werden sollen müssen Zeitintervalle angegeben werden.");
				return {};
			}

			else if (!Delays.empty() && (PinVec.size() == 1))
			{
				if (PinVec[0] == -1)
				{
					QMessageBox::warning(this, "Problem", "Eine Achse die 0V verwendet muss auch mindestens eine andere Stufe verwenden.");
					return {};
				}
			}

			else if (PinVec.empty())
			{
			   continue;
			}

			uniform_int_distribution<mt19937::result_type> gpiodist {0, PinVec.size() - 1};
			uniform_int_distribution<AxisDelays::size_type> delaydist {0, Delays.size() - 1};

			auto delay = Delays[delaydist(engine)];
			auto Pin = PinVec[gpiodist(engine)];

			if (MaxDelay != InvalidDelay)
			{
				if ((CurrentDelay + delay) == MaxDelay)
				{
					break;
				}

				else if ((CurrentDelay + delay) > MaxDelay)
				{
					delay = MaxDelay - CurrentDelay;
				}

				CurrentDelay += delay;
			}

			if (Pin != -1)
			{
                RPtr->add({Pin, !inverted, delay});
			}

			else
			{
				//-1 Pseudopin bedeuted abschalten aller anderen Pins derselben Achse
				if (PinVec.size() > 1)
				{
					for (auto & OtherPin : PinVec)
					{
						if (OtherPin != -1)
						{
                            RPtr->add({OtherPin, inverted, 0});
						}
					}

                    {
                        auto & somePin = RPtr->get_pin(RPtr->size()-1);
                        RPtr->add({somePin.PinNumber, inverted, delay}); //Irgendeinen Pin für ein delay einsetzen
                    }
				}

				else
				{
					continue;
				}
			}

			if (MaxDelay != InvalidDelay)
			{
				if (CurrentDelay >= MaxDelay)
				{
					break;
				}
			}

			if (MaxElements != -1)
			{
			   MaxElements--;
			}
		}

		//Wieder alles auf "Aus stellen"
		for (auto & Pins : PinCollection)
		{
			for (auto & GPIO : Pins)
			{
				if (GPIO != -1)
				{
                    RPtr->add({GPIO, inverted, 0});
				}
			}
		}

        return RPtr;
    }

    return {};
}

bool EasyGenDialog::apply(PiGPIOSequence::Sequence & )
{
	return false;
}

void EasyGenDialog::on_TimeAddButton_clicked()
{
	auto Delay = ui->TimeInputSpin->value();
	auto ListWidget = getCurrentAxisDelay();

	auto NewItem = new QListWidgetItem(ListWidget);
		NewItem->setText(QString::number(Delay));

	ListWidget->addItem(NewItem);
	m_DelaysMap[getCurrentAxisTabID()].emplace_back(Delay);
}

void EasyGenDialog::on_TimeSubButton_clicked()
{
	auto ListWidget = getCurrentAxisDelay();

	if (ListWidget->selectedItems().size() == 0)
	{
		QMessageBox::warning(this, "Problem", "Es wurde kein Element zum Entfernen ausgewaehlt");
	}

	auto Index = ListWidget->currentIndex().row();
	auto TabID = getCurrentAxisTabID();

	m_DelaysMap[TabID].erase(m_DelaysMap[TabID].begin() + Index);

	delete ListWidget->takeItem(Index);
}

void EasyGenDialog::on_MixButton_clicked()
{
	if (ui->MaxDelaySpin->value() == 0 && ui->MaxElemsSpin->value() == 0)
	{
		QMessageBox::warning(this, "Problem", "Es muss mindestens eine maximale Anzahl an Elementen oder eine maximale Dauer festgelegt werden.");
	}

	else
	{
		auto axisVoltsCheck = [](AxisPinInfo const & info) -> bool
		{
			for (auto & PinInfo : info)
			{
				if (std::get<2>(PinInfo)->isChecked())
				{
					return true;
				}
			}

			return false;
		};

		if (axisVoltsCheck(m_XInfo) ||
			axisVoltsCheck(m_YInfo) ||
			axisVoltsCheck(m_Z1Info) ||
			axisVoltsCheck(m_Z2Info))
		{
			auto delayListsOK = [this]() -> bool
			{
				for (auto & Pair : m_DelaysMap)
				{
					if (!Pair.second.empty())
					{
						return true;
					}
				}

				return false;
			};

			if (delayListsOK())
			{
				m_Success = true;
				close();
			}

			else
			{
				QMessageBox::warning(this, "Problem", "Es muss mindestens eine Elementdauer fuer mindestens eine Achse eingetragen werden");
			}
		}

		else
		{
			QMessageBox::warning(this, "Problem", "Es muss mindestens eine Spannung ausgewählt werden");
		}
	}
}

QListWidget * EasyGenDialog::getCurrentAxisDelay()
{
	switch (static_cast<AxisDelayTabs> (ui->AxisDelaysTabWidget->currentIndex()))
	{
		case AxisDelayTabs::X:
			return ui->XDelays;
		break;

		case AxisDelayTabs::Y:
			return ui->YDelays;
		break;

		case AxisDelayTabs::Z1:
			return ui->Z1Delays;
		break;

		case AxisDelayTabs::Z2:
			return ui->Z2Delays;
		break;
	}

	return nullptr;
}

QListWidget *EasyGenDialog::getAxisDelaysWidget(EasyGenDialog::AxisDelayTabs tabnum)
{
	switch (tabnum)
	{
		case AxisDelayTabs::X:
			return ui->XDelays;
		break;

		case AxisDelayTabs::Y:
			return ui->YDelays;
		break;

		case AxisDelayTabs::Z1:
			return ui->Z1Delays;
		break;

		case AxisDelayTabs::Z2:
			return ui->Z2Delays;
		break;
	}

	return nullptr;
}

EasyGenDialog::AxisDelayTabs EasyGenDialog::getCurrentAxisTabID() const
{
	return static_cast<AxisDelayTabs> (ui->AxisDelaysTabWidget->currentIndex());
}

void EasyGenDialog::on_TimeInputSpin_editingFinished()
{}

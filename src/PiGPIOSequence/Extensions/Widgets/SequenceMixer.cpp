/*
 *   Copyright (C) 2016 Anton Haubner <anton.haubner@outlook.de>
 *
 *   This file is part of the PiGPIOSequenceWidgets library.
 *
 *   PiGPIOSequenceWidgets is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU Affero General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   PiGPIOSequenceWidgets is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Affero General Public License for more details.
 *
 *   You should have received a copy of the GNU Affero General Public License
 *   along with PiGPIOSequenceWidgets.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <PiGPIOSequence/Extensions/Widgets/SequenceMixer.hpp>
#include "ui_SequenceMixer.h"

#include <PiGPIOSequence/Extensions/Widgets/PinSelector.hpp>

#include <QShortcut>

#include <random>
#include <limits>

using namespace std;

namespace PiGPIOSequence
{
    namespace Extensions
    {
        namespace Widgets
        {
            SequenceMixer::SequenceMixer(Sequence & seq, QWidget *parent) :
                QWidget(parent),
                ui(new Ui::SequenceMixer),
				m_Sequence(seq)
            {
                ui->setupUi(this);

				//Shortcuts
				auto DelShortcut = new QShortcut(QKeySequence(Qt::Key_Delete), ui->DelayList, nullptr, nullptr, Qt::WidgetShortcut);
				connect(DelShortcut, SIGNAL(activated()), this, SLOT(on_RemoveDelayButton_clicked()));

				auto AddShortcut = new QShortcut(QKeySequence(Qt::Key_Return), ui->DelaySpin, nullptr, nullptr, Qt::WidgetShortcut);
				connect(AddShortcut, SIGNAL(activated()), this, SLOT(on_AddDelayButton_clicked()));
            }

            SequenceMixer::~SequenceMixer()
            {
				delete ui;
			}

            Sequence const & SequenceMixer::getSequence() const noexcept
            {
                return m_Sequence;
            }

            void SequenceMixer::on_AddDelayButton_clicked()
            {
                ui->DelayList->addItem(QString::number(ui->DelaySpin->value()));
            }

            void SequenceMixer::on_RemoveDelayButton_clicked()
            {
                auto ItemList = ui->DelayList->selectedItems();

                for (decltype(ItemList)::size_type i = 0; i < ItemList.size(); ++i)
                {
                    delete ItemList.at(i);
                }
            }

            void SequenceMixer::on_MixButton_clicked()
            {
                m_Sequence.clear();

                random_device rd;
                mt19937 engine(rd());

                PinSelector::GPIONumList GPIOs;

                ui->PinSelector->getSelectedGPIOs(GPIOs);

                if (ui->GPIOsOnFromBeginCheck->isChecked())
                {
                    //Alle gewählten GPIOs von Beginn an AN
                    for (auto & GPIO : GPIOs)
                    {
                       m_Sequence.add({GPIO, true, 0});
                    }
                }

                if (   GPIOs.size() == 0
                    || ui->DelayList->count() == 0)
                {
                    emit MixingTried();
                    return;
                }

                uniform_int_distribution<mt19937::result_type> gpiodist {0, GPIOs.size() - 1};
                uniform_int_distribution<int> delaydist {0, ui->DelayList->count() - 1};

                Utils::Milliseconds delay;
                bool isInverted = ui->InvertSignalsBox->isChecked();


                int MaxElements = ui->MaxElementsSpin->value();
                    if (MaxElements == 0)
                    {
                        MaxElements = -1;
                    }

                Utils::Milliseconds MaxDelay = ui->MaxTimeSpin->value();
                    if (MaxDelay == 0)
                    {
                        MaxDelay = std::numeric_limits<Utils::Milliseconds>::max();
                    }

                Utils::Milliseconds CurrentDelay = 0;

                while (MaxElements > 0 ||
                       (MaxElements == -1 && MaxDelay != std::numeric_limits<Utils::Milliseconds>::max()))
                {
                    auto random_GPIONum = gpiodist(engine);
                    auto random_DelayIndex = delaydist(engine);

                    delay = ui->DelayList->item(random_DelayIndex)->text().toInt();

                    if (MaxDelay != std::numeric_limits<Utils::Milliseconds>::max())
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

                    m_Sequence.add({GPIOs[random_GPIONum], !isInverted, delay});
                    m_Sequence.add({GPIOs[random_GPIONum], isInverted, 0});

                    if (MaxDelay != std::numeric_limits<Utils::Milliseconds>::max())
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

                emit SequenceMixed();
            }
        }
    }
}

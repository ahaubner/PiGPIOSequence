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

#include "PulseGenDialog.hpp"
#include "ui_PulseGenDialog.h"

#include <QVBoxLayout>
#include <QMessageBox>

PulseGenDialog::PulseGenDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::PulseGenDialog),
	m_PinSelector(nullptr),
	m_Success(false)
{
	ui->setupUi(this);

	auto Layout = new QVBoxLayout(ui->PinSelectFrame);
		ui->PinSelectFrame->setLayout(Layout);

    m_PinSelector = new PiGPIOSequence::Extensions::Widgets::PinSelector(ui->PinSelectFrame);

	Layout->addWidget(m_PinSelector);
}

PulseGenDialog::~PulseGenDialog()
{
	if (m_PinSelector)
	{
		delete m_PinSelector;
	}

	delete ui;
}

SequenceGenerator::UniqueSequencePtr PulseGenDialog::generate()
{
	exec();

	UniqueSequencePtr Seq;

	if (m_Success)
	{
		m_Success = false;

		Seq.reset(new PiGPIOSequence::Sequence);

        auto OnDelay = static_cast<PiGPIOSequence::Utils::Milliseconds>(ui->ONDelay->value());
        auto OffDelay = static_cast<PiGPIOSequence::Utils::Milliseconds>(ui->OFFDelay->value());
		auto Repeat = ui->repeatCountSpin->value();

		auto invert = ui->InvertCheck->isChecked();

		auto Pin = m_SelectedGPIOs[0];

		for (decltype(Repeat) i = 0; i < Repeat; i++)
		{
            Seq->add({Pin, !invert, OnDelay});
            Seq->add({Pin, invert, OffDelay});
		}
	}

	return Seq;
}

bool PulseGenDialog::apply(PiGPIOSequence::Sequence &)
{
	return false;
}

void PulseGenDialog::on_OKButton_clicked()
{
	if (m_PinSelector)
	{
		m_PinSelector->getSelectedGPIOs(m_SelectedGPIOs);

		if (m_SelectedGPIOs.size() != 1)
		{
			QMessageBox::warning(this, "Problem", "Es muss nur ein Pin gewählt werden!");
		}

		else
		{
			m_Success = true;
			close();
		}
	}
}

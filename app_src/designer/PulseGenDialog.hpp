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

#ifndef PULSEGENDIALOG_HPP
#define PULSEGENDIALOG_HPP

#include <QDialog>

#include <PiGPIOSequence.hpp>
#include <PiGPIOSequence/Extensions/Widgets/PinSelector.hpp>

#include "SequenceGenerator.hpp"

namespace Ui
{
	class PulseGenDialog;
}

class PulseGenDialog
	:	public QDialog,
		public SequenceGenerator
{
		Q_OBJECT

    public:
		explicit PulseGenDialog(QWidget * parent = nullptr);
		virtual ~PulseGenDialog() override;

		virtual UniqueSequencePtr generate() override;
        virtual bool apply(PiGPIOSequence::Sequence &) override;

	private slots:
		void on_OKButton_clicked();

	private:
		Ui::PulseGenDialog * ui;

        PiGPIOSequence::Extensions::Widgets::PinSelector * m_PinSelector;
		bool m_Success;

        PiGPIOSequence::Extensions::Widgets::PinSelector::GPIONumList m_SelectedGPIOs;
};

#endif // PULSEGENDIALOG_HPP

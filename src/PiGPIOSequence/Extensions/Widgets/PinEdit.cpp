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

#include <PiGPIOSequence/Extensions/Widgets/PinEdit.hpp>
#include "ui_PinEdit.h"

namespace PiGPIOSequence
{
    namespace Extensions
    {
        namespace Widgets
        {
            PinEdit::PinEdit(PinConfig & Config, QWidget * parent) :
                QWidget(parent),
                ui(new Ui::PinEdit),
                m_PinConfig(Config)
            {
                ui->setupUi(this);

                updateUI();
            }

            PinEdit::~PinEdit()
            {
                delete ui;
            }

            PinConfig const & PinEdit::getConfig() const noexcept
            {
                return m_PinConfig;
            }

            void PinEdit::updateUI()
            {
                ui->PinNumSpin->setValue(m_PinConfig.PinNumber);
                ui->DelaySpin->setValue(m_PinConfig.delay);
                ui->IOCheck->setChecked(m_PinConfig.state);
            }

            void PinEdit::on_PinNumSpin_editingFinished()
            {
                m_PinConfig.PinNumber = ui->PinNumSpin->value();
                emit configEdited();
            }

            void PinEdit::on_DelaySpin_editingFinished()
            {
                m_PinConfig.delay = ui->DelaySpin->value();
                emit configEdited();
            }

            void PinEdit::on_IOCheck_clicked()
            {
                m_PinConfig.state = ui->IOCheck->isChecked();
                emit configEdited();
            }
        }
    }
}

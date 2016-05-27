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

#ifndef PIGPIOSEQUENCE_EXTENSIONS_WIDGETS_PINEDIT_HPP
#define PIGPIOSEQUENCE_EXTENSIONS_WIDGETS_PINEDIT_HPP

#include <QWidget>

#include <PiGPIOSequence.hpp>
#include <PiGPIOSequence/PinConfig.hpp>

namespace PiGPIOSequence
{
    namespace Extensions
    {
        namespace Widgets
        {
            namespace Ui
            {
                class PinEdit;
            }

            class PinEdit
                :   public QWidget
            {
                    Q_OBJECT

                public:
                    explicit PinEdit(PinConfig & Config, QWidget * parent = nullptr);
                    ~PinEdit();

                    PinConfig const & getConfig() const noexcept;

                signals:
                    void configEdited();

                private slots:
                    void on_PinNumSpin_editingFinished();
                    void on_DelaySpin_editingFinished();
                    void on_IOCheck_clicked();

                private:
                    void updateUI();

                    Ui::PinEdit * ui;

                    PinConfig & m_PinConfig;
            };
        }
    }
}

#endif // PIGPIOSEQUENCE_EXTENSIONS_WIDGETS_PINEDIT_HPP

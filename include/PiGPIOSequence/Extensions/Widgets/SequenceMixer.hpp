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

#ifndef PIGPIOSEQUENCE_EXTENSIONS_WIDGETS_SEQUENCEMIXER_HPP
#define PIGPIOSEQUENCE_EXTENSIONS_WIDGETS_SEQUENCEMIXER_HPP

#include <QWidget>

#include <PiGPIOSequence.hpp>

namespace PiGPIOSequence
{
    namespace Extensions
    {
        namespace Widgets
        {
            namespace Ui
            {
                class SequenceMixer;
            }

            class SequenceMixer
				:	public QWidget
            {
                    Q_OBJECT

                public:
                    explicit SequenceMixer(Sequence & seq, QWidget * parent = nullptr);
                    virtual ~SequenceMixer() override;

                    Sequence const & getSequence() const noexcept;

                signals:
                    void SequenceMixed();
					void MixingTried();

                private slots:

                    void on_AddDelayButton_clicked();

                    void on_RemoveDelayButton_clicked();

                    void on_MixButton_clicked();

                private:
                    Ui::SequenceMixer *ui;

                    Sequence & m_Sequence;
            };
        }
    }
}

#endif // PIGPIOSEQUENCEMIXERWIDGET_HPP

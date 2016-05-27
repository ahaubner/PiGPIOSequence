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

#ifndef PIGPIOSEQUENCE_EXTENSIONS_WIDGETS_SEQUENCELISTITEM_HPP
#define PIGPIOSEQUENCE_EXTENSIONS_WIDGETS_SEQUENCELISTITEM_HPP

#include <QWidget>

#include <PiGPIOSequence/Sequence.hpp>

namespace PiGPIOSequence
{
	namespace Extensions
	{
		namespace Widgets
		{
            namespace Ui
            {
                class SequenceListItem;
            }

            class SequenceListItem
				:	public QWidget
			{
					Q_OBJECT

				public:
                    explicit SequenceListItem(Sequence & seq, QWidget * parent = nullptr);
                    virtual ~SequenceListItem() override;

					void setName(QString const & name);
					QString getName() const;

                    Sequence const & getSequence() const noexcept;

				private slots:
					void on_EditorButton_clicked();

					void on_SaveButton_clicked();

					void on_LoadButton_clicked();

				private:
                    Ui::SequenceListItem * ui;

                    Sequence & m_Sequence;
			};
		}
	}
}

#endif

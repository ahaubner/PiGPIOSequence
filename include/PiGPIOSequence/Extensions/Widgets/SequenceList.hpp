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

#ifndef PIGPIOSEQUENCE_EXTENSIONS_WIDGETS_SEQUENCELIST_HPP
#define PIGPIOSEQUENCE_EXTENSIONS_WIDGETS_SEQUENCELIST_HPP

#include <QWidget>
#include <QListWidgetItem>

#include <PiGPIOSequence/ParallelSequences.hpp>

namespace PiGPIOSequence
{
	namespace Extensions
	{
        namespace Widgets
		{
            namespace Ui
            {
                class SequenceList;
            }

            class SequenceList
				:	public QWidget
			{
					Q_OBJECT

				public:
					using IndexList = std::vector<int>;

                    explicit SequenceList(ParallelSequences & PSequences, QWidget * parent = nullptr);
                    virtual ~SequenceList() override;

					void updateUI();

                    void addSequence(Sequence && seq);
					void eraseSequence(ParallelSequences::IndexType index);

					IndexList selectedSequences();

                    Sequence & accessSequence(ParallelSequences::IndexType i);
                    ParallelSequences const & getParallelSequences() const noexcept;

				private:
                    Ui::SequenceList * ui;

                    void addSequence_impl(Sequence & seq);

					void eraseListItem_impl(QListWidgetItem * Item);
					void clearUI_impl();

                    ParallelSequences & m_ParallelSequences;
			};
		}
	}
}

#endif

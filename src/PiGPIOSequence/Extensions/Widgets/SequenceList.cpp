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

#include <PiGPIOSequence/Extensions/Widgets/SequenceList.hpp>
#include "ui_SequenceList.h"

#include <PiGPIOSequence/Extensions/Widgets/SequenceListItem.hpp>

#include <cassert>

using namespace std;

namespace PiGPIOSequence
{
	namespace Extensions
	{
        namespace Widgets
		{
            SequenceList::SequenceList(ParallelSequences & PSequences, QWidget * parent) :
				QWidget(parent),
                ui(new Ui::SequenceList),
				m_ParallelSequences(PSequences)
			{
				ui->setupUi(this);

				updateUI();
			}

            SequenceList::~SequenceList()
			{
				clearUI_impl();

				delete ui;
			}

            void SequenceList::updateUI()
			{
				clearUI_impl();

                for (ParallelSequences::IndexType i = 0; i < m_ParallelSequences.size(); ++i)
                {
                    addSequence_impl(m_ParallelSequences.accessSequence(i));
                }
			}

            void SequenceList::addSequence(Sequence && seq)
			{
                m_ParallelSequences.addSequence(std::move(seq));

                updateUI();
			}

            void SequenceList::eraseSequence(ParallelSequences::IndexType index)
			{
                assert(index < m_ParallelSequences.size());
                m_ParallelSequences.erase(index);

                updateUI();
			}

            SequenceList::IndexList SequenceList::selectedSequences()
			{
				auto List = ui->SequenceListWidget->selectionModel()->selectedIndexes();
				IndexList RetList;

                for (decltype(List)::size_type i = 0; i < List.size(); ++i)
				{
					RetList.emplace_back(List.at(i).row());
				}

                return RetList;
            }

            Sequence & SequenceList::accessSequence(ParallelSequences::IndexType i)
            {
                assert(i < m_ParallelSequences.size());

                return m_ParallelSequences.accessSequence(i);
            }

            ParallelSequences const & SequenceList::getParallelSequences() const noexcept
			{
				return m_ParallelSequences;
			}

            void SequenceList::addSequence_impl(Sequence & seq)
			{
                auto Item = new QListWidgetItem(ui->SequenceListWidget);
                auto Widget = new SequenceListItem(seq, ui->SequenceListWidget);
				Item->setSizeHint(Widget->minimumSizeHint());

				ui->SequenceListWidget->addItem(Item);
				ui->SequenceListWidget->setItemWidget(Item, Widget);
			}

            void SequenceList::eraseListItem_impl(QListWidgetItem * Item)
			{
                auto Widget = static_cast<SequenceListItem *> (ui->SequenceListWidget->itemWidget(Item));

				delete Widget;
				delete Item;
			}

            void SequenceList::clearUI_impl()
			{
				while (ui->SequenceListWidget->count() > 0)
				{
					eraseListItem_impl(ui->SequenceListWidget->item(0));
				}
			}
		}
	}
}

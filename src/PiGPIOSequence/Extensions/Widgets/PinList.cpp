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

#include <PiGPIOSequence/Extensions/Widgets/PinList.hpp>
#include "ui_PinList.h"

#include <PiGPIOSequence/Extensions/Widgets/PinEdit.hpp>

using namespace std;

namespace PiGPIOSequence
{
    namespace Extensions
    {
        namespace Widgets
        {
            PinList::PinList(Sequence & sequence, QWidget * parent) :
                QWidget(parent),
                ui(new Ui::PinList),
                m_Sequence(sequence)
            {
                ui->setupUi(this);

                updateUI();
            }

            PinList::~PinList()
            {
                clearUI_impl();

                delete ui;
            }

            Sequence const & PinList::getSequence() const noexcept
            {
                return m_Sequence;
            }

            void PinList::updateUI()
            {
                clearUI_impl();

                for (Sequence::IndexType i = 0; i < m_Sequence.size(); ++i)
                {
                    addItem_impl(m_Sequence.access_pin(i));
                }
            }

            void PinList::addItem_impl(PinConfig & PinCfg)
            {
                QListWidgetItem * Item = new QListWidgetItem(ui->PinConfigList);
                ui->PinConfigList->addItem(Item);

                PinEdit * PinWidget = new PinEdit(PinCfg, ui->PinConfigList);
                Item->setSizeHint(PinWidget->minimumSizeHint());
                ui->PinConfigList->setItemWidget(Item, PinWidget);
            }

            void PinList::eraseListItem_impl(QListWidgetItem * Item)
            {
                auto Widget = static_cast<PinEdit *> (ui->PinConfigList->itemWidget(Item));

                delete Widget;
                delete Item;
            }

            void PinList::clearUI_impl()
            {
                while (ui->PinConfigList->count() > 0)
                {
                    eraseListItem_impl(ui->PinConfigList->item(0));
                }
            }

            void PinList::on_AddButton_clicked()
            {
                auto & pin = m_Sequence.add({2, false, 0});

                updateUI();
            }

            void PinList::on_SubButton_clicked()
            {
                auto ListWidget = ui->PinConfigList;
                auto SelectedItems = ListWidget->selectedItems();

                for (decltype(SelectedItems)::size_type i = 0; i < SelectedItems.size(); ++i)
                {
                    auto & Item = SelectedItems.at(i);

                    m_Sequence.erase(ListWidget->row(Item));
                    eraseListItem_impl(Item);
				}

                updateUI();
            }
        }
    }
}

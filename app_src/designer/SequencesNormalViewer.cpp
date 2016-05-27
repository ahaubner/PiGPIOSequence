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

#include "SequencesNormalViewer.hpp"

SequencesNormalViewer::SequencesNormalViewer(PiGPIOSequence::ParallelSequences & PSequences, QWidget * parent)
	:	m_Widget(nullptr)
{
    m_Widget = new PiGPIOSequence::Extensions::Widgets::SequenceList(PSequences, parent);
}

SequencesNormalViewer::~SequencesNormalViewer()
{

}

QWidget *SequencesNormalViewer::getWidget()
{
	return m_Widget;
}

void SequencesNormalViewer::addSequence()
{
    addSequence({});
}

void SequencesNormalViewer::addSequence(PiGPIOSequence::Sequence && seq)
{
	m_Widget->addSequence(std::move(seq));
}

void SequencesNormalViewer::eraseSequence()
{
	auto Selected = m_Widget->selectedSequences();

	if (!Selected.empty())
	{
		m_Widget->eraseSequence(Selected[0]);
	}
}

PiGPIOSequence::Sequence * SequencesNormalViewer::getSelected()
{
    PiGPIOSequence::Sequence * RetPtr = nullptr;

	auto Selected = m_Widget->selectedSequences();

	if (!Selected.empty())
	{
        auto & ParallelSeqs = m_Widget->getParallelSequences();

        if (Selected[0] < ParallelSeqs.size())
        {
            RetPtr = std::addressof(m_Widget->accessSequence(Selected[0]));
        }
	}

	return RetPtr;
}

void SequencesNormalViewer::updateUI()
{
	m_Widget->updateUI();
}


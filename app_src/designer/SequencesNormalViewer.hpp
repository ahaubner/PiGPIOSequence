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

#ifndef SEQUENCESNORMALVIEWER_HPP
#define SEQUENCESNORMALVIEWER_HPP

#include <QWidget>
#include "ParallelSequencesViewer.hpp"

#include <PiGPIOSequence/Extensions/Widgets/SequenceList.hpp>

class SequencesNormalViewer
	:	public ParallelSequencesViewer
{
	public:
        explicit SequencesNormalViewer(PiGPIOSequence::ParallelSequences & PSequences, QWidget * parent);
		virtual ~SequencesNormalViewer() override;

		virtual QWidget * getWidget() override;

		virtual void addSequence() override;
        virtual void addSequence(PiGPIOSequence::Sequence && seq) override;
		virtual void eraseSequence() override;

        virtual PiGPIOSequence::Sequence * getSelected() override;

	private:
		void updateUI();

        PiGPIOSequence::Extensions::Widgets::SequenceList * m_Widget;
};

#endif // SEQUENCESNORMALVIEWER_HPP

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

#ifndef PARALLELSEQUENCESVIEWER_HPP
#define PARALLELSEQUENCESVIEWER_HPP

#include <QWidget>

#include <PiGPIOSequence/ParallelSequences.hpp>

class ParallelSequencesViewer
{
	public:
		explicit ParallelSequencesViewer();
		virtual ~ParallelSequencesViewer();

		virtual QWidget * getWidget() = 0;

		virtual void addSequence() = 0;
        virtual void addSequence(PiGPIOSequence::Sequence && seq) = 0;
		virtual void eraseSequence() = 0;

        virtual PiGPIOSequence::Sequence * getSelected() = 0;
};

#endif // PARALLELSEQUENCESVIEWER_HPP

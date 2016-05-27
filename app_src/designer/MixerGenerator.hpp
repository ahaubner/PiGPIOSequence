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

#ifndef MIXERGENERATOR_HPP
#define MIXERGENERATOR_HPP

#include <QDialog>

#include <PiGPIOSequence/Extensions/Widgets/SequenceMixer.hpp>

#include "SequenceGenerator.hpp"

class MixerGenerator
	:	public SequenceGenerator
{
	public:
		MixerGenerator(QWidget * parent);
		virtual ~MixerGenerator() override;

        virtual SequenceGenerator::UniqueSequencePtr generate() override;
        virtual bool apply(PiGPIOSequence::Sequence &) override;

	private:
		QDialog * m_Diag;
        PiGPIOSequence::Extensions::Widgets::SequenceMixer * m_MixerWidget;
        PiGPIOSequence::Sequence m_SeqBuffer;
};

#endif

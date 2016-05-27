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

#include "MixerGenerator.hpp"

#include <QVBoxLayout>

MixerGenerator::MixerGenerator(QWidget * parent)
	:	m_Diag(nullptr),
		m_MixerWidget(nullptr)
{
	m_Diag = new QDialog(parent);
    m_MixerWidget = new PiGPIOSequence::Extensions::Widgets::SequenceMixer(m_SeqBuffer, m_Diag);

	QVBoxLayout * Layout = new QVBoxLayout(m_Diag);
		Layout->addWidget(m_MixerWidget);
		m_Diag->setLayout(Layout);
		//m_Diag->setFixedSize(512, 512);
		m_Diag->setWindowTitle("Mixer Generator");

	QObject::connect(m_MixerWidget, SIGNAL(MixingTried()), m_Diag, SLOT(close()));
    QObject::connect(m_MixerWidget, SIGNAL(SequenceMixed()), m_Diag, SLOT(close()));
}

MixerGenerator::~MixerGenerator()
{
	if (m_MixerWidget)
	{
		delete m_MixerWidget;
	}

	if (m_Diag)
	{
		delete m_Diag;
	}
}

SequenceGenerator::UniqueSequencePtr MixerGenerator::generate()
{
    UniqueSequencePtr Ptr;

	if (m_Diag)
	{
		if (m_MixerWidget)
        {
            m_Diag->exec();

            if (m_SeqBuffer.size() > 0)
            {
                Ptr.reset(new PiGPIOSequence::Sequence(m_SeqBuffer));
            }
		}
	}

    return Ptr;
}

bool MixerGenerator::apply(PiGPIOSequence::Sequence &)
{
	return false;
}


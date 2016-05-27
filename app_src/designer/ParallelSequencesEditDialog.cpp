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

#include "ParallelSequencesEditDialog.hpp"
#include "ui_ParallelSequencesEditDialog.h"

#include <stdexcept>

#include <QMessageBox>
#include <QHBoxLayout>

#include "SequencesNormalViewer.hpp"
#include "MixerGenerator.hpp"
#include "PulseGenDialog.hpp"
#include "EasyGenDialog.hpp"

using namespace std;

ParallelSequencesEditDialog::ParallelSequencesEditDialog(PiGPIOSequence::ParallelSequences & PSequences, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ParallelSequencesEditDialog),
	m_CurrentViewWidget(nullptr),
	m_ParallelSequences(PSequences)
{
	ui->setupUi(this);

	m_ViewerLayout = new QHBoxLayout(ui->ViewWidgetFrame);
	ui->ViewWidgetFrame->setLayout(m_ViewerLayout);

	m_Generators[Generators::Easy] = std::unique_ptr<SequenceGenerator>(new EasyGenDialog(this));
	m_Generators[Generators::Mixer] = std::unique_ptr<SequenceGenerator>(new MixerGenerator(this));
	m_Generators[Generators::Pulse] = std::unique_ptr<SequenceGenerator>(new PulseGenDialog(this));

	updateUI();
}

ParallelSequencesEditDialog::~ParallelSequencesEditDialog()
{
	if (m_CurrentViewWidget)
	{
		if (m_CurrentViewWidget->getWidget())
		{
			delete m_CurrentViewWidget->getWidget();
		}

		delete m_CurrentViewWidget;
	}

	delete ui;
}

PiGPIOSequence::ParallelSequences const & ParallelSequencesEditDialog::getSequences() const noexcept
{
	return m_ParallelSequences;
}

void ParallelSequencesEditDialog::updateUI()
{
    setupViewer_impl();
}

void ParallelSequencesEditDialog::setupViewer_impl()
{
	if (m_CurrentViewWidget)
	{
		if (m_CurrentViewWidget->getWidget())
		{
			delete m_CurrentViewWidget->getWidget();
		}

		delete m_CurrentViewWidget;
		m_CurrentViewWidget = nullptr;
	}

	switch (static_cast<Viewers>(ui->ViewSelection->currentIndex()))
	{
		case Viewers::Normal:
			m_CurrentViewWidget = new SequencesNormalViewer(m_ParallelSequences, ui->ViewWidgetFrame);
		break;

		default:
		break;
	}

	if (m_CurrentViewWidget)
	{
		m_ViewerLayout->addWidget(m_CurrentViewWidget->getWidget());
	}
}

ParallelSequencesEditDialog::GeneratorPtr &ParallelSequencesEditDialog::getGenerator_impl()
{
	auto key = static_cast<Generators> (ui->GeneratorSelection->currentIndex());

	if (m_Generators.find(key) == m_Generators.end())
	{
		throw runtime_error("Generator is not registered");
	}

	return m_Generators[key];
}

void ParallelSequencesEditDialog::on_AddSequenceButton_clicked()
{
	if (m_CurrentViewWidget)
	{
		m_CurrentViewWidget->addSequence();
	}
}

void ParallelSequencesEditDialog::on_EraseSequenceButton_clicked()
{
	if (m_CurrentViewWidget)
	{
		m_CurrentViewWidget->eraseSequence();
	}
}

void ParallelSequencesEditDialog::on_GenerateButton_clicked()
{
	if (m_CurrentViewWidget)
	{
		auto & Gen = getGenerator_impl();

		auto Seq = Gen->generate();

		if (Seq)
		{
            m_CurrentViewWidget->addSequence(std::move(*Seq));
		}

		else
		{
			QMessageBox::warning(this, "Problem", "Der Generator konnte keine Sequenz erstellen");
		}
	}
}

void ParallelSequencesEditDialog::on_ExtendButton_clicked()
{
	if (m_CurrentViewWidget)
	{
		auto Selected = m_CurrentViewWidget->getSelected();

        if (Selected)
		{
			auto & Gen = getGenerator_impl();

            if (Gen->apply(*Selected))
			{
				QMessageBox::information(this, "Fertig", "Generator auf Sequenz angewandt");
			}

			else
			{
				QMessageBox::warning(this, "Problem", "Der Generator konnte nichts an der Sequenz ändern");
			}
		}

		else
		{
			QMessageBox::warning(this, "Problem", "Keine Sequenz ausgewählt!");
		}
	}
}

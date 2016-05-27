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

#ifndef PARALLELSEQUENCESEDITDIALOG_HPP
#define PARALLELSEQUENCESEDITDIALOG_HPP

#include <QDialog>
#include <QHBoxLayout>

#include <PiGPIOSequence.hpp>

#include "ParallelSequencesViewer.hpp"
#include "SequenceGenerator.hpp"

#include <memory>

namespace Ui
{
	class ParallelSequencesEditDialog;
}

class ParallelSequencesEditDialog
	:	public QDialog
{
		Q_OBJECT

    public:
        explicit ParallelSequencesEditDialog(PiGPIOSequence::ParallelSequences & PSequences, QWidget * parent = nullptr);
		virtual ~ParallelSequencesEditDialog() override;

        PiGPIOSequence::ParallelSequences const & getSequences() const noexcept;

	private slots:
		void on_AddSequenceButton_clicked();
		void on_EraseSequenceButton_clicked();
		void on_GenerateButton_clicked();
		void on_ExtendButton_clicked();

	private:
		enum class Viewers : int
		{
			Normal = 0
		};

		enum class Generators : int
		{
			Easy = 0,
			Mixer = 1,
			Pulse = 2
		};

		using GeneratorPtr = std::unique_ptr<SequenceGenerator>;
		using GeneratorMap = std::map<Generators, GeneratorPtr>;

		void updateUI();

		void setupViewer_impl();
		GeneratorPtr & getGenerator_impl();

		Ui::ParallelSequencesEditDialog *ui;

		QHBoxLayout * m_ViewerLayout;

		ParallelSequencesViewer * m_CurrentViewWidget;
        PiGPIOSequence::ParallelSequences & m_ParallelSequences;

		GeneratorMap m_Generators;
};

#endif // PARALLELSEQUENCESEDITDIALOG_HPP

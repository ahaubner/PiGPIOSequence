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

#ifndef PARALLELSEQUENCECONTAINERWIDGET_HPP
#define PARALLELSEQUENCECONTAINERWIDGET_HPP

#include <QWidget>

#include <PiGPIOSequence/ParallelSequences.hpp>

#include "PSequenceMeta.hpp"

namespace Ui
{
	class ParallelSequencesContainerWidget;
}

class ParallelSequenceContainerWidget
	:	public QWidget
{
		Q_OBJECT

	public:
        explicit ParallelSequenceContainerWidget(PSequenceMeta & seq, QWidget * parent = nullptr);
		virtual ~ParallelSequenceContainerWidget() override;

		void setName(QString const & name);
		QString getName() const;

        PiGPIOSequence::ParallelSequences const & getSequences() const noexcept;

	signals:
        void onEdit(PiGPIOSequence::ParallelSequences & pseq);

	private slots:
		void on_EditorButton_clicked();

		void on_SaveButton_clicked();

		void on_LoadButton_clicked();

        void on_NameEdit_editingFinished();

    private:
		Ui::ParallelSequencesContainerWidget *ui;

        PSequenceMeta & m_ParallelSequences;
};

#endif // PARALLELSEQUENCECONTAINERWIDGET_HPP

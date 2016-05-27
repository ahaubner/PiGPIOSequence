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

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QTimer>

#include <PiGPIOSequence/Extensions/Networking.hpp>
#include <PiGPIOSequence.hpp>

#include "PSequenceMeta.hpp"

//Teile der UI werde äußerst langsam, wenn ein Element entfernt oder neu erstellt wird,
//da die Widgets direkt per Referenz auf den PiGPIOSequence Objekten operieren,
//denn immer wenn der Vektor zu dem das unterliegende Objekt gehört geändert wird, können alle Referenzen invalidiert werden,
//was dazu führt, dass alle Widgets neu erstellt werden müssen.
//
//TODO: Referenzen durch Indizes oder ähnliches ersetzen!

namespace Ui
{
	class MainWindow;
}

class MainWindow
	:	public QMainWindow
{
		Q_OBJECT

    public:
		explicit MainWindow(QWidget * parent = nullptr);
		virtual ~MainWindow() override;

	private slots:
        void onPSeqEdit(PiGPIOSequence::ParallelSequences & PSeq);
		void on_AddSequenceButton_clicked();
		void on_RemoveSequenceButton_clicked();
		void on_SendButton_clicked();
		void on_InfoButton_clicked();
		void on_FindIPButton_clicked();
		void on_IPInput_returnPressed();

		void update();

		void on_ConnectButton_clicked();
		void on_StatusRequestButton_clicked();
		void on_SequenceStopButton_clicked();

		void on_DisconnectButton_clicked();

	private:
		void setConnected(bool onOff);
		void refreshStatus();

        void updatePSeqListUI();
        void addPSeqListItem_impl(PSequenceMeta & pSeqMeta);

		Ui::MainWindow *ui;

		PiGPIOSequence::Extensions::Networking::Client m_Client;

		bool m_IsConnected;
		bool m_ClientWork;


        QTimer m_UpdateTimer;

        using ParallelSequencesVector = std::vector<PSequenceMeta>;

		ParallelSequencesVector m_ParallelSequences;
};

#endif // MAINWINDOW_HPP

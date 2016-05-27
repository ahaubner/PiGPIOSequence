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

#ifndef EASYGENDIALOG_HPP
#define EASYGENDIALOG_HPP

#include <tuple>
#include <vector>

#include <QDialog>
#include <QListWidget>
#include <QCheckBox>

#include <PiGPIOSequence.hpp>

#include "SequenceGenerator.hpp"

namespace Ui
{
	class EasyGenDialog;
}

class EasyGenDialog
	:	public QDialog,
		public SequenceGenerator
{
		Q_OBJECT

	public:
		explicit EasyGenDialog(QWidget *parent = nullptr);
		virtual ~EasyGenDialog() override;

        virtual SequenceGenerator::UniqueSequencePtr generate() override;
        virtual bool apply(PiGPIOSequence::Sequence &) override;

	private slots:
		void on_TimeAddButton_clicked();

		void on_TimeSubButton_clicked();

		void on_MixButton_clicked();

		void on_TimeInputSpin_editingFinished();

	private:
		using PinEnableInfo = std::tuple<PiGPIOSequence::Utils::PinNumber, unsigned int, QCheckBox *>;
		using AxisPinInfo = std::vector<PinEnableInfo>;
		using AxisDelays = std::vector<PiGPIOSequence::Utils::Milliseconds>;

		enum class AxisDelayTabs : int
		{
			X = 0,
			Y = 1,
			Z1 = 2,
			Z2 = 3
		};

		using AxisDelaysMap = std::map<AxisDelayTabs, AxisDelays>;

		QListWidget * getCurrentAxisDelay();
		QListWidget * getAxisDelaysWidget(AxisDelayTabs tabnum);
		AxisDelayTabs getCurrentAxisTabID() const;

		Ui::EasyGenDialog * ui;

		AxisPinInfo m_XInfo;
		AxisPinInfo m_YInfo;
		AxisPinInfo m_Z1Info;
		AxisPinInfo m_Z2Info;

		AxisDelaysMap m_DelaysMap;

		bool m_Success;
};

#endif // EASYGENDIALOG_HPP

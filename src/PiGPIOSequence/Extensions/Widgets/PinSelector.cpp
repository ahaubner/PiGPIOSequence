/*
 *   Copyright (C) 2016 Anton Haubner <anton.haubner@outlook.de>
 *
 *   This file is part of the PiGPIOSequenceWidgets library.
 *
 *   PiGPIOSequenceWidgets is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU Affero General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   PiGPIOSequenceWidgets is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Affero General Public License for more details.
 *
 *   You should have received a copy of the GNU Affero General Public License
 *   along with PiGPIOSequenceWidgets.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <PiGPIOSequence/Extensions/Widgets/PinSelector.hpp>
#include "ui_PinSelector.h"

#define PIN_ACTION(PinLabel) PinClicked(PinLabel, ui-> PinLabel -> isChecked())

namespace PiGPIOSequence
{
    namespace Extensions
    {
        namespace Widgets
        {
            PinSelector::PinSelector(QWidget * parent) :
                QWidget(parent),
                ui(new Ui::PinSelector)
            {
                ui->setupUi(this);

                //Map vorbelegen
                for (Utils::PinNumber i = V3_3_N0; i <= GPIO21; ++i)
                {
                    m_PinMap[static_cast<Pins>(i)] = false;
                }
            }

            PinSelector::~PinSelector()
            {
                delete ui;
            }

            const PinSelector::PinMap &PinSelector::getPinStates() const noexcept
            {
                return m_PinMap;
            }

            void PinSelector::getSelectedPins(PinSelector::PinList & ReturnList) const
            {
                ReturnList.clear();

                for (PinMap::const_iterator i = m_PinMap.begin(); i != m_PinMap.end(); ++i)
                {
                    if (i->second)
                    {
                        ReturnList.push_back(i->first);
                    }
                }
            }

            void PinSelector::getSelectedGPIOs(PinSelector::GPIONumList & ReturnList) const
            {
                ReturnList.clear();

				Utils::PinNumber GPIONum = -1;

                for (PinMap::const_iterator i = m_PinMap.begin(); i != m_PinMap.end(); ++i)
                {
                    if (i->second)
                    {
                        switch (i->first)
                        {
                            case GPIO2:
                                GPIONum = 2;
                            break;

                            case GPIO3:
                                GPIONum = 3;
                            break;

                            case GPIO4:
                                GPIONum = 4;
                            break;

                            case GPIO5:
                                GPIONum = 5;
                            break;

                            case GPIO6:
                                GPIONum = 6;
                            break;

                            case GPIO7:
                                GPIONum = 7;
                            break;

                            case GPIO8:
                                GPIONum = 8;
                            break;

                            case GPIO9:
                                GPIONum = 9;
                            break;

                            case GPIO10:
                                GPIONum = 10;
                            break;

                            case GPIO11:
                                GPIONum = 11;
                            break;

                            case GPIO12:
                                GPIONum = 12;
                            break;

                            case GPIO13:
                                GPIONum = 13;
                            break;

                            case GPIO14:
                                GPIONum = 14;
                            break;

                            case GPIO15:
                                GPIONum = 15;
                            break;

                            case GPIO16:
                                GPIONum = 16;
                            break;

                            case GPIO17:
                                GPIONum = 17;
                            break;

                            case GPIO18:
                                GPIONum = 18;
                            break;

                            case GPIO19:
                                GPIONum = 19;
                            break;

                            case GPIO20:
                                GPIONum = 20;
                            break;

                            case GPIO21:
                                GPIONum = 21;
                            break;

                            case GPIO22:
                                GPIONum = 22;
                            break;

                            case GPIO23:
                                GPIONum = 23;
                            break;

                            case GPIO24:
                                GPIONum = 24;
                            break;

                            case GPIO25:
                                GPIONum = 25;
                            break;

                            case GPIO26:
                                GPIONum = 26;
                            break;

							case GPIO27:
								GPIONum = 27;
							break;

							default:
								GPIONum = -1;
							break;
                        }

                        if (GPIONum != -1)
                        {
							ReturnList.emplace_back(GPIONum);
                            GPIONum = -1;
                        }
                    }
				}
            }

            void PinSelector::PinClicked(PinSelector::Pins Pin, bool state)
            {
				bool origState = m_PinMap[Pin];
                m_PinMap[Pin] = state;

                if (origState != state)
                {
                    emit pinsChanged();
                }
            }

            void PinSelector::on_V3_3_N0_clicked()
            {
                PIN_ACTION(V3_3_N0);
            }

            void PinSelector::on_GPIO2_clicked()
            {
                PIN_ACTION(GPIO2);
            }

            void PinSelector::on_V5_N0_clicked()
            {
                PIN_ACTION(V5_N0);
            }

            void PinSelector::on_V5_N1_clicked()
            {
                PIN_ACTION(V5_N1);
            }

            void PinSelector::on_GPIO3_clicked()
            {
                PIN_ACTION(GPIO3);
            }

            void PinSelector::on_GPIO14_clicked()
            {
                PIN_ACTION(GPIO14);
            }

            void PinSelector::on_GROUND_N0_clicked()
            {
                PIN_ACTION(GROUND_N0);
            }

            void PinSelector::on_GPIO4_clicked()
            {
                PIN_ACTION(GPIO4);
            }

            void PinSelector::on_GROUND_N1_clicked()
            {
                PIN_ACTION(GROUND_N1);
            }

            void PinSelector::on_GPIO15_clicked()
            {
                PIN_ACTION(GPIO15);
            }

            void PinSelector::on_GPIO17_clicked()
            {
                PIN_ACTION(GPIO17);
            }

            void PinSelector::on_GPIO18_clicked()
            {
                PIN_ACTION(GPIO18);
            }

            void PinSelector::on_GPIO27_clicked()
            {
                PIN_ACTION(GPIO27);
            }

            void PinSelector::on_GROUND_N2_clicked()
            {
                PIN_ACTION(GROUND_N2);
            }

            void PinSelector::on_GPIO22_clicked()
            {
                PIN_ACTION(GPIO22);
            }

            void PinSelector::on_GPIO23_clicked()
            {
                PIN_ACTION(GPIO23);
            }

            void PinSelector::on_V3_3_N1_clicked()
            {
                PIN_ACTION(V3_3_N1);
            }

            void PinSelector::on_GPIO24_clicked()
            {
                PIN_ACTION(GPIO24);
            }

            void PinSelector::on_GPIO10_clicked()
            {
                PIN_ACTION(GPIO10);
            }

            void PinSelector::on_GROUND_N3_clicked()
            {
                PIN_ACTION(GROUND_N3);
            }

            void PinSelector::on_GPIO9_clicked()
            {
                PIN_ACTION(GPIO9);
            }

            void PinSelector::on_GPIO25_clicked()
            {
                PIN_ACTION(GPIO25);
            }

            void PinSelector::on_GPIO11_clicked()
            {
                PIN_ACTION(GPIO11);
            }

            void PinSelector::on_GPIO8_clicked()
            {
                PIN_ACTION(GPIO8);
            }

            void PinSelector::on_GROUND_N4_clicked()
            {
                PIN_ACTION(GROUND_N4);
            }

            void PinSelector::on_GPIO7_clicked()
            {
                PIN_ACTION(GPIO7);
            }

            void PinSelector::on_ID_SD_clicked()
            {
                PIN_ACTION(ID_SD);
            }

            void PinSelector::on_ID_SC_clicked()
            {
                PIN_ACTION(ID_SC);
            }

            void PinSelector::on_GPIO5_clicked()
            {
                PIN_ACTION(GPIO5);
            }

            void PinSelector::on_GROUND_N5_clicked()
            {
                PIN_ACTION(GROUND_N5);
            }

            void PinSelector::on_GPIO6_clicked()
            {
                PIN_ACTION(GPIO6);
            }

            void PinSelector::on_GPIO12_clicked()
            {
                PIN_ACTION(GPIO12);
            }

            void PinSelector::on_GPIO13_clicked()
            {
                PIN_ACTION(GPIO13);
            }

            void PinSelector::on_GROUND_N6_clicked()
            {
                PIN_ACTION(GROUND_N6);
            }

            void PinSelector::on_GPIO19_clicked()
            {
                PIN_ACTION(GPIO19);
            }

            void PinSelector::on_GPIO16_clicked()
            {
                PIN_ACTION(GPIO16);
            }

            void PinSelector::on_GPIO26_clicked()
            {
                PIN_ACTION(GPIO26);
            }

            void PinSelector::on_GPIO20_clicked()
            {
                PIN_ACTION(GPIO20);
            }

            void PinSelector::on_GROUND_N7_clicked()
            {
                PIN_ACTION(GROUND_N7);
            }

            void PinSelector::on_GPIO21_clicked()
            {
                PIN_ACTION(GPIO21);
            }
        }
    }
}

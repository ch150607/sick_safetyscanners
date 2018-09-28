// this is for emacs file handling -*- mode: c++; indent-tabs-mode: nil -*-

// -- BEGIN LICENSE BLOCK ----------------------------------------------

/*!
*  Copyright (C) 2018, SICK AG, Waldkirch
*  Copyright (C) 2018, FZI Forschungszentrum Informatik, Karlsruhe, Germany
*
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*    http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.

*/

// -- END LICENSE BLOCK ------------------------------------------------

//----------------------------------------------------------------------
/*!
 * \file ChangeCommSettingsCommand.cpp
 *
 * \author  Lennart Puck <puck@fzi.de>
 * \date    2018-09-24
 */
//----------------------------------------------------------------------

#include <sick_microscan3_ros_driver/cola2/ChangeCommSettingsCommand.h>

#include <sick_microscan3_ros_driver/cola2/Cola2Session.h>
#include <sick_microscan3_ros_driver/cola2/Command.h>

namespace sick {
namespace cola2 {

ChangeCommSettingsCommand::ChangeCommSettingsCommand(Cola2Session& session,
                                                     sick::datastructure::CommSettings settings)
  : MethodCommand(session, 0x00b0)
  , m_settings(settings)
{
  m_writer_ptr = boost::make_shared<sick::data_processing::ReadWriteHelper>();
}

void ChangeCommSettingsCommand::addTelegramData(
  sick::datastructure::PacketBuffer::VectorBuffer& telegram) const
{
  base_class::addTelegramData(telegram);

  BYTE* data_ptr = prepareTelegramAndGetDataPtr(telegram);

  writeDataToDataPtr(data_ptr);
}

BYTE* ChangeCommSettingsCommand::prepareTelegramAndGetDataPtr(
  sick::datastructure::PacketBuffer::VectorBuffer& telegram) const
{
  UINT16 prevSize = telegram.size();
  telegram.resize(prevSize + 28);
  return telegram.data() + prevSize;
}

bool ChangeCommSettingsCommand::writeDataToDataPtr(BYTE*& data_ptr) const
{
  writeChannelToDataPtr(data_ptr);
  writeEnabledToDataPtr(data_ptr);
  writeEInterfaceTypeToDataPtr(data_ptr);
  writeIPAdresstoDataPtr(data_ptr);
  writePortToDataPtr(data_ptr);
  writeFrequencyToDataPtr(data_ptr);
  writeStartAngleToDataPtr(data_ptr);
  writeEndAngleToDataPtr(data_ptr);
  writeFeaturesToDataPtr(data_ptr);
}

bool ChangeCommSettingsCommand::canBeExecutedWithoutSessionID() const
{
  return true;
}

bool ChangeCommSettingsCommand::processReply()
{
  if (!base_class::processReply())
  {
    return false;
  }
  return true;
}

bool ChangeCommSettingsCommand::writeChannelToDataPtr(BYTE*& data_ptr) const
{
  m_writer_ptr->writeUINT8LittleEndian(data_ptr, m_settings.getChannel(), 0);
}

bool ChangeCommSettingsCommand::writeEnabledToDataPtr(BYTE*& data_ptr) const
{
  m_writer_ptr->writeUINT8LittleEndian(data_ptr, m_settings.getEnabled(), 4);
}

bool ChangeCommSettingsCommand::writeEInterfaceTypeToDataPtr(BYTE*& data_ptr) const
{
  m_writer_ptr->writeUINT8LittleEndian(data_ptr, m_settings.getEInterfaceType(), 5);
}

bool ChangeCommSettingsCommand::writeIPAdresstoDataPtr(BYTE*& data_ptr) const
{
  m_writer_ptr->writeUINT32LittleEndian(data_ptr, m_settings.getHostIp().to_ulong(), 8);
}

bool ChangeCommSettingsCommand::writePortToDataPtr(BYTE*& data_ptr) const
{
  m_writer_ptr->writeUINT16LittleEndian(data_ptr, m_settings.getHostUdpPort(), 12);
}

bool ChangeCommSettingsCommand::writeFrequencyToDataPtr(BYTE*& data_ptr) const
{
  m_writer_ptr->writeUINT16LittleEndian(data_ptr, m_settings.getPublishingFequency(), 14);
}

bool ChangeCommSettingsCommand::writeStartAngleToDataPtr(BYTE*& data_ptr) const
{
  m_writer_ptr->writeUINT32LittleEndian(data_ptr, m_settings.getStartAngle(), 16);
}

bool ChangeCommSettingsCommand::writeEndAngleToDataPtr(BYTE*& data_ptr) const
{
  m_writer_ptr->writeUINT32LittleEndian(data_ptr, m_settings.getEndAngle(), 20);
}

bool ChangeCommSettingsCommand::writeFeaturesToDataPtr(BYTE*& data_ptr) const
{
  m_writer_ptr->writeUINT16LittleEndian(data_ptr, m_settings.getFeatures(), 24);
}


} // namespace cola2
} // namespace sick

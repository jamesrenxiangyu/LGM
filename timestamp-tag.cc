/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Authors: Joe Kopena <tjkopena@cs.drexel.edu>
 *
 * These applications are used in the WiFi Distance Test experiment,
 * described and implemented in test02.cc.  That file should be in the
 * same place as this file.  The applications have two very simple
 * jobs, they just generate and receive packets.  We could use the
 * standard Application classes included in the NS-3 distribution.
 * These have been written just to change the behavior a little, and
 * provide more examples.
 *
 */

#include <ostream>

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"

#include "ns3/stats-module.h"

#include "wifi-example-apps.h"

using namespace ns3;

//----------------------------------------------------------------------
//-- TimestampTag
//------------------------------------------------------
TypeId 
TimestampTag::GetTypeId (void)
{
  static TypeId tid = TypeId ("TimestampTag")
    .SetParent<Tag> ()
    .AddConstructor<TimestampTag> ()
    .AddAttribute ("Timestamp",
                   "Some momentous point in time!",
                   EmptyAttributeValue (),
                   MakeTimeAccessor (&TimestampTag::GetTimestamp),
                   MakeTimeChecker ())
  ;
  return tid;
}
TypeId 
TimestampTag::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

uint32_t 
TimestampTag::GetSerializedSize (void) const
{
  return 8;
}
void 
TimestampTag::Serialize (TagBuffer i) const
{
  int64_t t = m_timestamp.GetNanoSeconds ();
  i.Write ((const uint8_t *)&t, 8);
}
void 
TimestampTag::Deserialize (TagBuffer i)
{
  int64_t t;
  i.Read ((uint8_t *)&t, 8);
  m_timestamp = NanoSeconds (t);
}

void
TimestampTag::SetTimestamp (Time time)
{
  m_timestamp = time;
}
Time
TimestampTag::GetTimestamp (void) const
{
  return m_timestamp;
}

void 
TimestampTag::Print (std::ostream &os) const
{
  os << "t=" << m_timestamp;
}

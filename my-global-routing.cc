
//
// Network topology
//
//  n0 --------- n3 ---------- n6
//  |            |             | 
//  |            |             |
//  n1 --------- n4 ---------- n7
//  |            |             | 
//  |            |             |
//  n2 --------- n5 ---------- n8
//

#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/ipv4-global-routing-helper.h"

#include "ns3/netanim-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("SimpleGlobalRoutingExample");


int 
main (int argc, char *argv[])
{

  //LogComponentEnable("GlobalRouter", LOG_LEVEL_LOGIC);
  //LogComponentEnable("GlobalRouteManagerImpl", LOG_LEVEL_LOGIC);
  //LogComponentEnable("GlobalRouteManager", LOG_LEVEL_LOGIC);
  LogComponentEnable("Ipv4RoutingProtocol", LOG_LEVEL_LOGIC);

  // Users may find it convenient to turn on explicit debugging
  // for selected modules; the below lines suggest how to do this
#if 0 
  LogComponentEnable ("SimpleGlobalRoutingExample", LOG_LEVEL_INFO);
#endif

  // Set up some default values for the simulation.  Use the 
  Config::SetDefault ("ns3::OnOffApplication::PacketSize", UintegerValue (210));
  Config::SetDefault ("ns3::OnOffApplication::DataRate", StringValue ("448kb/s"));

  //DefaultValue::Bind ("DropTailQueue::m_maxPackets", 30);

  // Allow the user to override any of the defaults and the above
  // DefaultValue::Bind ()s at run-time, via command-line arguments
  CommandLine cmd (__FILE__);
  bool enableFlowMonitor = false;
  cmd.AddValue ("EnableMonitor", "Enable Flow Monitor", enableFlowMonitor);
  cmd.Parse (argc, argv);

  // Here, we will explicitly create four nodes.  In more sophisticated
  // topologies, we could configure a node factory.
  NS_LOG_INFO ("Create nodes.");
  NodeContainer c;
  c.Create (9);
  NodeContainer n0n1 = NodeContainer (c.Get (0), c.Get (1));
  NodeContainer n1n2 = NodeContainer (c.Get (1), c.Get (2));
  NodeContainer n3n4 = NodeContainer (c.Get (3), c.Get (4));
  NodeContainer n4n5 = NodeContainer (c.Get (4), c.Get (5));
  NodeContainer n6n7 = NodeContainer (c.Get (6), c.Get (7));
  NodeContainer n7n8 = NodeContainer (c.Get (7), c.Get (8));

  NodeContainer n0n3 = NodeContainer (c.Get (0), c.Get (3));
  NodeContainer n3n6 = NodeContainer (c.Get (3), c.Get (6));
  NodeContainer n1n4 = NodeContainer (c.Get (1), c.Get (4));
  NodeContainer n4n7 = NodeContainer (c.Get (4), c.Get (7));
  NodeContainer n2n5 = NodeContainer (c.Get (2), c.Get (5));
  NodeContainer n5n8 = NodeContainer (c.Get (5), c.Get (8));

  InternetStackHelper internet;
  internet.Install (c);

  // We create the channels first without any IP addressing information
  NS_LOG_INFO ("Create channels.");
  PointToPointHelper p2p;
  p2p.SetQueue("ns3::DropTailQueue");
  p2p.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  p2p.SetChannelAttribute ("Delay", StringValue ("2ms"));
  NetDeviceContainer d0d1 = p2p.Install (n0n1);
  NetDeviceContainer d1d2 = p2p.Install (n1n2);
  NetDeviceContainer d3d4 = p2p.Install (n3n4);
  NetDeviceContainer d4d5 = p2p.Install (n4n5);
  NetDeviceContainer d6d7 = p2p.Install (n6n7);
  NetDeviceContainer d7d8 = p2p.Install (n7n8);

  NetDeviceContainer d0d3 = p2p.Install (n0n3);
  NetDeviceContainer d3d6 = p2p.Install (n3n6);
  NetDeviceContainer d1d4 = p2p.Install (n1n4);
  NetDeviceContainer d4d7 = p2p.Install (n4n7);
  NetDeviceContainer d2d5 = p2p.Install (n2n5);
  NetDeviceContainer d5d8 = p2p.Install (n5n8);

  // Later, we add IP addresses.
  NS_LOG_INFO ("Assign IP Addresses.");
  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer i0i1 = ipv4.Assign (d0d1);
  i0i1.SetMetric (0, 3);
  i0i1.SetMetric(1, 3);

  ipv4.SetBase ("10.1.2.0", "255.255.255.0");
  Ipv4InterfaceContainer i1i2 = ipv4.Assign (d1d2);
  i1i2.SetMetric(0, 5);
  i1i2.SetMetric(1, 5);

  ipv4.SetBase ("10.1.3.0", "255.255.255.0");
  Ipv4InterfaceContainer i3i4 = ipv4.Assign (d3d4);
  i3i4.SetMetric(0, 6);
  i3i4.SetMetric(1, 6);

  ipv4.SetBase ("10.1.4.0", "255.255.255.0");
  Ipv4InterfaceContainer i4i5 = ipv4.Assign (d4d5);
  i4i5.SetMetric (0, 7);
  i4i5.SetMetric (1, 7);

  ipv4.SetBase ("10.1.5.0", "255.255.255.0");
  Ipv4InterfaceContainer i6i7 = ipv4.Assign (d6d7);
  i6i7.SetMetric(0, 8);
  i6i7.SetMetric(1, 8);

  ipv4.SetBase ("10.1.6.0", "255.255.255.0");
  Ipv4InterfaceContainer i7i8 = ipv4.Assign (d7d8);
  i7i8.SetMetric(0,9);
  i7i8.SetMetric(1,9);


  ipv4.SetBase ("10.1.7.0", "255.255.255.0");
  Ipv4InterfaceContainer i0i3 = ipv4.Assign (d0d3);
  i0i3.SetMetric (0, 10);
  i0i3.SetMetric (1, 10);

  ipv4.SetBase ("10.1.8.0", "255.255.255.0");
  Ipv4InterfaceContainer i3i6 = ipv4.Assign (d3d6);
  i3i6.SetMetric(0,11);
  i3i6.SetMetric(1,11);

  ipv4.SetBase ("10.1.9.0", "255.255.255.0");
  Ipv4InterfaceContainer i1i4 = ipv4.Assign (d1d4);
  i1i4.SetMetric(0,12);
  i1i4.SetMetric(1,12);

  ipv4.SetBase ("10.1.10.0", "255.255.255.0");
  Ipv4InterfaceContainer i4i7 = ipv4.Assign (d4d7);
  i4i7.SetMetric (0, 13);
  i4i7.SetMetric (1, 13);

  ipv4.SetBase ("10.1.11.0", "255.255.255.0");
  Ipv4InterfaceContainer i2i5 = ipv4.Assign (d2d5);
  i2i5.SetMetric(0,14);
  i2i5.SetMetric(1,14);

  ipv4.SetBase ("10.1.12.0", "255.255.255.0");
  Ipv4InterfaceContainer i5i8 = ipv4.Assign (d5d8);
  i5i8.SetMetric(0,15);
  i5i8.SetMetric(1,15);

  // Create router nodes, initialize routing database and set up the routing
  // tables in the nodes.
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
  // pointToPoint.EnablePcapAll("second");
  Ipv4GlobalRoutingHelper g;
  Ptr<OutputStreamWrapper> routingStream = Create<OutputStreamWrapper>
                                            ("global-routing.routes", std::ios::out);
  g.PrintRoutingTableAllAt (Seconds (10), routingStream);

// Create the OnOff application to send UDP datagrams of size
  // 210 bytes at a rate of 448 Kb/s
  NS_LOG_INFO ("Create Applications.");
  uint16_t port = 9;   // Discard port (RFC 863)
  OnOffHelper onoff ("ns3::UdpSocketFactory", 
                     Address (InetSocketAddress (i5i8.GetAddress (1), port)));
  onoff.SetConstantRate (DataRate ("448kb/s"));
  ApplicationContainer apps = onoff.Install (c.Get (0));
  apps.Start (Seconds (1.0));
  apps.Stop (Seconds (10.0));

  // Create a packet sink to receive these packets
  PacketSinkHelper sink ("ns3::UdpSocketFactory",
                         Address (InetSocketAddress (Ipv4Address::GetAny (), port)));
  apps = sink.Install (c.Get (8));
  apps.Start (Seconds (1.0));
  apps.Stop (Seconds (10.0));

  AsciiTraceHelper ascii;
  p2p.EnableAsciiAll (ascii.CreateFileStream ("my-global-routing.tr"));
  p2p.EnablePcapAll ("my-global-routing");

  // Flow Monitor
  FlowMonitorHelper flowmonHelper;
  if (enableFlowMonitor)
    {
      flowmonHelper.InstallAll ();
    }

  NS_LOG_LOGIC ("Create Animation interface..");
  AnimationInterface anim ("my-global-routing-nimation.xml");
  anim.SetConstantPosition(c.Get(0), 10.0, 10.0);
  anim.SetConstantPosition(c.Get(1), 10.0, 40.0);
  anim.SetConstantPosition(c.Get(2), 10.0, 70.0);
  anim.SetConstantPosition(c.Get(3), 40.0, 10.0);
  anim.SetConstantPosition(c.Get(4), 40.0, 40.0);
  anim.SetConstantPosition(c.Get(5), 40.0, 70.0);
  anim.SetConstantPosition(c.Get(6), 70.0, 10.0);
  anim.SetConstantPosition(c.Get(7), 70.0, 40.0);
  anim.SetConstantPosition(c.Get(8), 70.0, 70.0);

  NS_LOG_INFO ("Run Simulation.");
  Simulator::Stop (Seconds (11));
  Simulator::Run ();
  NS_LOG_INFO ("Done.");

  if (enableFlowMonitor)
    {
      flowmonHelper.SerializeToXmlFile ("my-global-routing.flowmon", false, false);
    }

  Simulator::Destroy ();
  return 0;
}

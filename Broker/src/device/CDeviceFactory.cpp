////////////////////////////////////////////////////////////////////////////////
/// @file       CDeviceFactory.cpp
///
/// @author     Thomas Roth <tprfh7@mst.edu>
/// @author     Michael Catanzaro <michael.catanzaro@mst.edu>
///
/// @project    FREEDM DGI
///
/// @description
///     Handles the creation of devices and their structures.
///
/// @copyright
///     These source code files were created at Missouri University of Science
///     and Technology, and are intended for use in teaching or research. They
///     may be freely copied, modified, and redistributed as long as modified
///     versions are clearly marked as such and this notice is not removed.
///     Neither the authors nor Missouri S&T make any warranty, express or
///     implied, nor assume any legal responsibility for the accuracy,
///     completeness, or usefulness of these files or any information
///     distributed with these files. 
///     
///     Suggested modifications or questions about these files can be directed
///     to Dr. Bruce McMillin, Department of Computer Science, Missouri
///     University of Science and Technology, Rolla, MO 65409 <ff@mst.edu>.
////////////////////////////////////////////////////////////////////////////////

#include <boost/foreach.hpp>

#include "device/CDeviceFactory.hpp"
#include "device/CGenericAdapter.hpp"
#include "device/CPscadAdapter.hpp"
#include "device/CRtdsAdapter.hpp"
#include "device/types/CDeviceFid.hpp"
#include "config.hpp"

#define foreach BOOST_FOREACH

namespace freedm {
namespace broker {
namespace device {

namespace {

/// This file's logger.
CLocalLogger Logger(__FILE__);

}

////////////////////////////////////////////////////////////////////////////////
/// Retrieves the singleton factory instance.
///
/// @pre None.
/// @post Nothing happens. This function only provides access to the factory.
///
/// @return the factory instance.
///
/// @limitations Be sure CDeviceFactory::init has been called on the factory
///  before doing anything with it.
////////////////////////////////////////////////////////////////////////////////
CDeviceFactory& CDeviceFactory::instance()
{
    Logger.Trace << __PRETTY_FUNCTION__ << std::endl;
    static CDeviceFactory instance;
    return instance;
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
////////////////////////////////////////////////////////////////////////////////
/// Initializes the device factory with a device manager and connection
/// information for its device adapter(s). This function should be called once, 
/// before the factory is ever used.  For example:
/// <code>CDeviceFactory::instance().init(params)</code>
///
/// @SharedMemory The factory keeps a reference to the passed device manager and
///  IO service, which are shared among other program modules.
///
/// @pre Relevant parameters are set appropriately. If RTDS is enabled, the
///  filename of the fpga message specification must be set in the global
///  configuration.
/// @post CDeviceFactory::instance() will retrieve the factory instance.
///
/// @param manager the device manager with which this factory should register
///  newly-created devices. This manager MUST remain a valid reference, unless
///  the factory is reinitialized.
/// @param ios if PSCAD or RTDS is enabled, the IO service for the line client.
/// @param host if PSCAD or RTDS is enabled, the hostname of the machine that
///  runs the simulation.
/// @param port if PSCAD or RTDS is enabled, the port number this DGI and the
///  simulation communicate with.
/// @todo update and figure out what parameters this function ought to have ...
///
/// @limitations Must be called before anything else is done with this factory.
////////////////////////////////////////////////////////////////////////////////
void CDeviceFactory::init(CPhysicalDeviceManager::Pointer manager,
        boost::asio::io_service& ios, const std::string fpgaCfgFile,
        const std::string host, const std::string port)
{
    Logger.Trace << __PRETTY_FUNCTION__ << std::endl;
    Logger.Info << "Initialized the device factory" << std::endl;
    m_manager = manager;
#if defined USE_DEVICE_PSCAD
    CPscadAdapter::Pointer pscadAdapter = CPscadAdapter::Create(ios);
    pscadAdapter->Connect(host, port);
    m_adapter = pscadAdapter;
#elif defined USE_DEVICE_RTDS
    CRtdsAdapter::Pointer rtdsAdapter = CRtdsAdapter::Create(
            ios, fpgaCfgFile, "rtds");
    rtdsAdapter->Connect(host, port);
    rtdsAdapter->Run();
    m_adapter = rtdsAdapter;
    m_ios = &ios;
    m_fpgaCfgFile = fpgaCfgFile;
#else
    m_adapter = CGenericAdapter::Create();
#endif
    m_initialized = true;
}
#pragma GCC diagnostic warning "-Wunused-parameter"

////////////////////////////////////////////////////////////////////////////////
/// Registers a device creation function with the factory under the specified 
/// string key. The key for the function should be the name of the class,
/// less the "CDevice" prefix. To simplify usage of this function, use the
/// REGISTER_DEVICE_CLASS macro as explained in the class comment block.
///
/// @ErrorHandling Throws an exception if the key has already been registered.
///
/// @pre None, presuming the suggested naming convention is followed.
/// @post The device class is now registered in the factory, allowing the
///  factory to create devices of this class with just a string representing the
///  name of the class.
///
/// @param key the string for the function to be associated with.
/// @param value the function to be associated with the string key.
///
/// @limitations None. This function can, and probably must, be called before
///  the factory is configured with init.
////////////////////////////////////////////////////////////////////////////////
void CDeviceFactory::RegisterDeviceClass(const std::string key,
        const FactoryFunction value)
{
    Logger.Trace << __PRETTY_FUNCTION__ << std::endl;

    if (m_registry.count(key) != 0)
    {
        std::stringstream ss;
        ss << "Attempted to register device factory function for class "
                << key << ", which has already been registered.";
        throw std::runtime_error(ss.str());
    }

    m_registry.insert(std::make_pair(key, value));
    Logger.Info << "Registered device class " << key << std::endl;
}


////////////////////////////////////////////////////////////////////////////////
/// Translates a string into a class type, then creates a new device of this 
/// type with the specified identifier.
///
/// @ErrorHandling Throws an exception if the device type is not registered
///  with the factory, or if the factory is uninitialized.
///
/// @pre The factory has been configured with CDeviceFactory::init.
/// @post Specified device is created and registered with the factory's device
///  manager.
///
/// @param deviceID the unique identifier for the device to be created.
///  No other device on this DGI may have this ID.
/// @param deviceType a string representing the name of the IDevice subclass
///  be created. Should be exactly the same as the portion of the class name
///  after "CDevice".
///
/// @limitations Device classes must properly register themselves before
///  instances of that device class can be constructed.
////////////////////////////////////////////////////////////////////////////////
void CDeviceFactory::CreateDevice(const Identifier deviceID,
        const std::string deviceType)
{
    Logger.Trace << __PRETTY_FUNCTION__ << std::endl;

    if (!m_initialized)
    {
        std::stringstream ss;
        ss << __PRETTY_FUNCTION__ << " called before factory init" << std::endl;
        throw std::runtime_error(ss.str());
    }

    // Ensure the specified device type exists
    if (m_registry.find(deviceType) == m_registry.end())
    {
        std::stringstream ss;
        ss << "Attempted to create device of unregistered type "
                << deviceType.c_str();
        throw std::runtime_error(ss.str());
    }

    // *m_registry[deviceType] is a member function.
    // Call it with the argument deviceID...
    ( this->*m_registry[deviceType] )( deviceID );
}

////////////////////////////////////////////////////////////////////////////////
/// Creates all devices specified by the passed vector. The vector is intended
/// to be generated in main by a configuration file (e.g. freedm.cfg). Devices 
/// should be of the mandatory format name:type (e.g. sst1:SST).
///
/// @ErrorHandling Exceptions are thrown if one of the strings does not follow
///  the correct format, if two devices are created with the same name, if
///  a device does not have a type specified, if a requested device type has not
///  been registered with the factory, or if the factory is uninitialized.
///
/// @pre The factory has been configured with CDeviceFactory::init.
/// @post Specified devices are created and registered with the factory's device
///  manager.
///
/// @param deviceList a list of device specifications in the above format.
///
/// @limitations main must call RegisterPhysicalDevices before this function.
////////////////////////////////////////////////////////////////////////////////
void CDeviceFactory::CreateDevices(const std::vector<std::string>& deviceList)
{
    Logger.Trace << __PRETTY_FUNCTION__ << std::endl;

    if (!m_initialized)
    {
        std::stringstream ss;
        ss << __PRETTY_FUNCTION__ << " called before factory init" << std::endl;
        throw std::runtime_error(ss.str());
    }
    foreach(std::string device, deviceList)
    {
        size_t colon = device.find(':');

        if (colon == std::string::npos)
        {
            std::stringstream ss;
            ss << "Incorrect device specification: " << device;
            throw std::runtime_error(ss.str());
        }

        std::string name(device.begin(), device.begin() + colon);
        std::string type(device.begin() + colon + 1, device.end());

        if (m_manager->DeviceExists(name))
        {
            std::stringstream ss;
            ss << "Specified duplicate device " << device;
            throw std::runtime_error(ss.str());
        }
        else if (type.empty())
        {
            std::stringstream ss;
            ss << "No type specified for device " << device;
            throw std::runtime_error(ss.str());
        }
        else
        {
            CreateDevice(name, type);
            Logger.Info << "Added " << type << ": " << name << std::endl;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
/// Constructs the factory. Should only ever be called from 
/// CDeviceFactory::instance.
///
/// @pre None.
/// @post Nothing happens. This function only provides access to the factory.
///
/// @return the factory instance.
///
/// @limitations Be sure CDeviceFactory::init has been called on the factory
///  before doing anything with it.
////////////////////////////////////////////////////////////////////////////////
CDeviceFactory::CDeviceFactory()
: m_adapter(), m_manager(), m_registry(), m_initialized(false)
{
    Logger.Trace << __PRETTY_FUNCTION__ << std::endl;
}


#ifdef USE_DEVICE_RTDS

///
/// Temporary hack to get FIDs working.
template <>
void CDeviceFactory::CreateDevice<CDeviceFid>(const Identifier deviceID)
{
    if (!m_initialized)
    {
        std::stringstream ss;
        ss << __PRETTY_FUNCTION__ << " called before factory init" << std::endl;
        throw std::runtime_error(ss.str());
    }

    CRtdsAdapter::Pointer adapter =
            CRtdsAdapter::Create(*m_ios, m_fpgaCfgFile, deviceID);

    // Connect and start the adapter
    boost::property_tree::ptree xmlTree;
    read_xml(m_fpgaCfgFile, xmlTree);
    std::string host, port;
    try
    {
        host = xmlTree.get<std::string > ( deviceID + ".host" );
        port = xmlTree.get<std::string > ( deviceID + ".port" );
    }
    catch (...)
    {
        throw std::runtime_error("Couldn't read hostname or port for FID "
                + deviceID);
    }
    adapter->Connect(host, port);
    adapter->Run();

    // Attach the adapter to a new FID and register it with the device manager.
    m_manager->AddDevice(IDevice::Pointer(new CDeviceFid(deviceID, adapter)));
}
#endif

} // namespace device
} // namespace freedm
} // namespace broker

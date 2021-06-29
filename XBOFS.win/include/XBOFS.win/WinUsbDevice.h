#pragma once
#include <XBOFS.win/pch.h>
#include <XBOFS.win/constants.h>
#include <ViGEm/Client.h>
#include <qobject.h>
#include <qstring.h>
#include <qregularexpression.h>
#include <qsettings.h>
#include <optional>

namespace XBOFSWin {

    std::optional<std::wstring> getWinUsbStringDescriptor(const WINUSB_INTERFACE_HANDLE &winUsbHandle, UCHAR index, USHORT languageId);

    class WinUsbDevice : public QObject
    {
        Q_OBJECT

    public:        
        WinUsbDevice(std::wstring devicePath, std::shared_ptr<spdlog::logger> logger, QObject* parent=nullptr);
        ~WinUsbDevice() {};        

    public slots:
        void run();
        void refreshSettings();

    signals:
        void vigEmConnect(const std::wstring &devicePath);
        void vigEmConnected(const std::wstring &devicePath);
        void vigEmTargetAdd(const std::wstring &devicePath);
        void vigEmTargetAdded(const std::wstring &devicePath);        
        void vigEmTargetInfo(const std::wstring &devicePath, quint16 vendorId, quint16 productId, const ulong index);
        void vigEmError(const std::wstring &devicePath);
        void winUsbDeviceOpen(const std::wstring &devicePath);
        void winUsbDeviceInfo(const std::wstring &devicePath, const QString &vendorId, const QString &vendorName,
                              const QString &productId, const QString &productName, const QString &serialNumber);
        void winUsbDeviceOpened(const std::wstring &devicePath);
        void winUsbDeviceInit(const std::wstring &devicePath);
        void winUsbDeviceInitComplete(const std::wstring &devicePath);
        void winUsbDeviceReadingInput(const std::wstring &devicePath);
        void winUsbDeviceTerminating(const std::wstring &devicePath);
        void winUsbDeviceError(const std::wstring &devicePath);

        void debuggingInfoButtonState(const XBO_ARCADE_STICK_BUTTON_STATE &buttonState);
        void debuggingInfoControllerState(const XUSB_REPORT &controllerState);
        void debuggingInfoPerformanceData(quint16 lastInnerLoopTime, quint16 worstInnerLoopTime, quint16 averageInnerLoopTime, quint16 bestInnerLoopTime);

    protected:        
        const std::shared_ptr<spdlog::logger> logger;
        const std::wstring devicePath;
        const QSettings settings;
        QString vendorId = "";
        QString vendorName = "";
        QString productId = "";
        QString productName = "";
        QString serialNumber = "";
        GLOBAL_INPUT_STATE state = GLOBAL_INPUT_STATE::GUIDE_UP;
        bool bindingEnabled = false;
        bool debuggingEnabled = false;

        bool buttons[15] = {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false};

        bool deviceHandlesOpen = false;
        UCHAR XBO_ARCADE_STICK_INIT[5] = { 0x05, 0x20, 0x00, 0x01, 0x00 };
        XBO_ARCADE_STICK_DATA_PACKET dataPacket = {};
        XBO_ARCADE_STICK_BUTTON_STATE buttonState = {};
        XBO_ARCADE_STICK_BUTTON_STATE previousButtonState = {};
        PVIGEM_CLIENT vigEmClient = NULL;
        PVIGEM_TARGET vigEmTarget = NULL;
        WINUSB_INTERFACE_HANDLE winUsbHandle;
        HANDLE deviceHandle;        

        bool openDevice();
        bool closeDevice();
        bool initXBOArcadeStick();
        bool readInputFromXBOArcadeStick();
        PACKET_TYPES processInputFromXBOArcadeStick();
        XUSB_REPORT prepareInputForVigEmController();
        bool dispatchInputToVigEmController();
    };
}


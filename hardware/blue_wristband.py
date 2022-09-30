# -*- coding: utf-8 -*-
"""
@Time: 2022/4/10 19:33
@Author: Veera
@Version: 3.9.0
@File: blue_wristband.py.py
@Software: PyCharm
"""

"""
  public static final UUID TX_POWER_UUID = UUID.fromString("00001804-0000-1000-8000-00805f9b34fb");
    public static final UUID TX_POWER_LEVEL_UUID = UUID.fromString("00002a07-0000-1000-8000-00805f9b34fb");
    public static final UUID CCCD = UUID.fromString("00002902-0000-1000-8000-00805f9b34fb");
    public static final UUID FIRMWARE_REVISON_UUID = UUID.fromString("00002a26-0000-1000-8000-00805f9b34fb");
    public static final UUID DIS_UUID = UUID.fromString("0000180a-0000-1000-8000-00805f9b34fb");
    public static final UUID RX_SERVICE_UUID = UUID.fromString("0000fff0-0000-1000-8000-00805f9b34fb");

    //注意，这里的RX对于DA14580来说的接收，也就是0xfff1
    public static final UUID RX_CHAR_UUID = UUID.fromString("0000fff1-0000-1000-8000-00805f9b34fb");
    //注意，这里的TX对于DA14580来说的发送，也就是0xfff4，notify方式。
    public static final UUID TX_CHAR_UUID = UUID.fromString("0000fff4-0000-1000-8000-00805f9b34fb");
"""
"""
80:EA:CA:03:02:01: fk_watch1.1
"""



import asyncio
import sys

from bleak import BleakScanner, BleakClient
from bleak.backends.scanner import AdvertisementData
from bleak.backends.device import BLEDevice

UART_SERVICE_UUID = "0000fff0-0000-1000-8000-00805f9b34fb"
UART_RX_CHAR_UUID = "0000fff1-0000-1000-8000-00805f9b34fb"
UART_TX_CHAR_UUID = "0000fff4-0000-1000-8000-00805f9b34fb"

# All BLE devices have MTU of at least 23. Subtracting 3 bytes overhead, we can
# safely send 20 bytes at a time to any device supporting this service.
UART_SAFE_SIZE = 20


async def uart_terminal():
    """This is a simple "terminal" program that uses the Nordic Semiconductor
    (nRF) UART service. It reads from stdin and sends each line of data to the
    remote device. Any data received from the device is printed to stdout.
    """
    def match_nus_uuid(device: BLEDevice, adv: AdvertisementData):
        if UART_SERVICE_UUID.lower() in adv.service_uuids:
            return True
        return False
    device = await BleakScanner.find_device_by_filter(match_nus_uuid)

    def handle_disconnect(_: BleakClient):
        print("Device was disconnected, goodbye.")
        # cancelling all tasks effectively ends the program
        for task in asyncio.all_tasks():
            task.cancel()
    def handle_rx(_: int, data: bytearray):
        print("received:", data)
    address = "80:EA:CA:03:02:01"
    async with BleakClient(address) as client:
        print(client.is_connected)
        await client.start_notify(UART_TX_CHAR_UUID, handle_rx)
        print("Connected, start typing and press ENTER...")
        loop = asyncio.get_running_loop()
        while True:
            data = await loop.run_in_executor(None, sys.stdin.buffer.readline)
            if not data:
                break
            await client.write_gatt_char(UART_RX_CHAR_UUID, data)
            print("sent:", data)


if __name__ == "__main__":


    try:
        asyncio.run(uart_terminal())
    except asyncio.CancelledError:
        # task is cancelled on disconnect, so we ignore this error
        pass
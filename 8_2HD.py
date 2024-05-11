import argparse
import asyncio
import RPi.GPIO as GPIO
from HCSR04_python_lib import HCSR04
from bleak import BleakScanner
from bleak import BleakClient

# Setting up GPIO
GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)

# Setting up Ultrasonic
TRIG = 23
ECHO = 24
GPIO.setup(TRIG,GPIO.OUT)
GPIO.setup(ECHO,GPIO.IN)
HCSR = HCSR04(trigger_pin=TRIG, echo_pin=ECHO)


LED_UUID = '19b10000-e8f2-537e-4f6c-d104768a1214'
CHAR_UUID = '19B10001-E8F2-537E-4F6C-D104768A1214'
   
#Main function
async def main():
    while True:
        print("Searching Arduino Nano 33 IoT")
        # Scan BLE devices for timeout seconds and return discovered devices with advertising data
        devices = await BleakScanner.discover(timeout=5, return_adv=True)
        for ble_device, adv_data in devices.values():
            if ble_device.name == 'LED':
                print("Device found")
                # Connect to Arduino Nano 33 IoT
                #If connected
                try:
                    async with BleakClient(ble_device.address) as client:
                        print("Connected to device")
                        while True:
                            distance = HCSR.get_distance(sample_size=5, decimal_places=0)
                            distance = int(distance)
                            
                            # Send Distance
                            print(distance)
                            val = await client.write_gatt_char(CHAR_UUID, (distance).to_bytes(1, byteorder='big'))

                            # Wait
                            await asyncio.sleep(0.5)
                #If not connected
                except:
                    await client.disconnect()
                    print("Device was disconnected, goodbye.")
                    
if __name__ == "__main__":
    asyncio.run(main())

#!/usr/bin/env python3
"""
FreeD Protocol Receiver

A simple Python implementation of the FreeD protocol receiver for camera tracking data.
FreeD is a UDP-based protocol used for transmitting camera position and lens data
in broadcast and film production environments.

Usage:
    python3 freed_receiver.py [port]

Author: ZCAM Team
License: No License
"""

import socket
import time
import datetime
import sys

def get_local_ip():
    """Get local IP address for display purposes"""
    try:
        # Create a temporary UDP socket to get local IP
        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        s.connect(('8.8.8.8', 80))  # Connect to an external address (using Google's DNS server)
        ip = s.getsockname()[0]  # Get local IP
        s.close()
        return ip
    except Exception:
        return "127.0.0.1"  # Return localhost if failed to get IP

def fmt_be24(u32):
    """Parse 24-bit big-endian value"""
    value = (u32[0] << 16 | u32[1] << 8 | u32[2])
    return value

def fmt_be24_15(u32):
    """Parse 24-bit big-endian signed value with 15-bit precision"""
    value = ((-1 << 24) | u32[0] << 16 | u32[1] << 8 | u32[2])
    if not (value & 0x00800000):
        value &= 0x00ffffff
    return value / 32768.0

def fmt_be24_6(u32):
    """Parse 24-bit big-endian signed value with 6-bit precision"""
    value = ((-1 << 24) | u32[0] << 16 | u32[1] << 8 | u32[2])
    if not (value & 0x00800000):
        value &= 0x00ffffff
    return value / 64.0

def format_zoom_to_focal_length(code):
    """Format zoom value as focal length in mm"""
    return f"{code / 100.0:.3f}(mm)"

def format_focus_distance(code):
    """Format focus value as distance in mm"""
    return f"{code / 10.0:.3f}(mm)"

def format_iris(code):
    """Format iris value as f-number"""
    return f"F{code / 100.0:.1f}"

def validate_checksum(data):
    """Validate FreeD packet checksum"""
    if len(data) < 29:
        return False
        
    checksum = int(data[28])
    if checksum == 0:
        return True  # Some manufacturers don't use checksum
        
    cs = 0x40
    for i in range(0, 28):
        val = int(data[i])
        cs = (cs - val) % 256
        
    return cs == checksum

def unpack_free_d(data):
    """Parse FreeD packet data to dictionary"""
    if len(data) < 29:
        print(f"Warning: Packet too short ({len(data)} bytes)")
        return None

    if data[0] != 0xD1:
        print(f"Warning: Invalid header byte 0x{data[0]:02X}")
        return None

    # Validate checksum
    if not validate_checksum(data):
        print("Warning: Checksum validation failed")
        return None

    try:
        info = {}
        info['CamID']   = int(data[1])
        info['Pan']     = round(fmt_be24_15(data[2:5]), 6)
        info['Tilt']    = round(fmt_be24_15(data[5:8]), 6)
        info['Roll']    = round(fmt_be24_15(data[8:11]), 6)
        
        # Optional: Parse position data (X, Y, Z)
        # info['X']       = round(fmt_be24_6(data[11:14]), 6)
        # info['Y']       = round(fmt_be24_6(data[14:17]), 6)
        # info['Z']       = round(fmt_be24_6(data[17:20]), 6)
        
        # Parse lens data
        zoom_code = fmt_be24(data[20:23])
        focus_code = fmt_be24(data[23:26])
        info['Zoom']    = format_zoom_to_focal_length(zoom_code)
        info['Focus']   = format_focus_distance(focus_code)
        
        # Parse iris and sequence
        iris = ((data[26] & 0x0f) << 8) | (data[27] & 0xff)
        info['Iris']    = format_iris(iris)
        info['seq']     = (data[26] & 0xf0) >> 4
        
        return info
        
    except Exception as e:
        print(f"Error parsing packet: {e}")
        return None

def main():
    """Main function"""
    # Get port from command line argument or use default
    port = int(sys.argv[1]) if len(sys.argv) > 1 else 5001
    
    # Create UDP socket
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    s.bind(('', port))
    
    # Get and display local IP
    local_ip = get_local_ip()
    print(f"### Local IP Address: {local_ip} ###")
    print(f"### FreeD Server is listening on: {local_ip}:{port} ###")
    print("### Press Ctrl+C to stop ###")
    print()
    
    seq = 0
    try:
        while True:
            data, address = s.recvfrom(4096)
            seq += 1
            
            freed_data = unpack_free_d(data)
            if freed_data:
                print(f"[{datetime.datetime.now()}] from {address}: {freed_data}")
            else:
                print(f"[{datetime.datetime.now()}] from {address}: Invalid packet")
                
    except KeyboardInterrupt:
        print("\n### FreeD Receiver stopped ###")
    except Exception as e:
        print(f"Error: {e}")
    finally:
        s.close()

if __name__ == "__main__":
    main()

# Example usage:
#   python3 zcam_freed_receiver.py
#   python3 zcam_freed_receiver.py 5002
#
# Example output:
#   {'CamID': 1, 'Pan': -1.299988, 'Tilt': 30.0, 'Roll': 0.0, 
#    'Zoom': '180.000(mm)', 'Focus': '10000.000(mm)', 'Iris': 'F4.0', 'seq': 0}
#
# API Control (if supported by your system):
#   curl "http://192.168.9.18/ctrl/freed?enable=1&camera_id=002&ip=192.168.9.102&port=5001"
#   curl "http://192.168.9.18/ctrl/freed?enable=0"

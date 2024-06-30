import socket
import struct
from ctypes import *

# Server address and port
server_address = '192.168.1.42'
server_port = 8080

class PlayerData(Structure):
    _fields_ = [
        ("player_id", c_uint8),
        ("state", c_uint8),
        ("mv_dir", c_uint8),
        ("atk_dir", c_uint8),
        ("pos_x", c_float),
        ("pos_y", c_float),
        ("name", c_char * 4)
    ]

class Player:
    def __init__(self, player_id):
        self.data = PlayerData()
        self.data.player_id = player_id
        self.data.state = 0  # 0: alive, 1: dead, 2: crouch, 3: jump
        self.data.mv_dir = 0  # 0: down, 1: right, 2: left, 3: up
        self.data.atk_dir = 0  # 0: not attacking, 1: right, 2: left, 3: up
        self.data.pos_x = 0.0
        self.data.pos_y = 0.0
        self.data.name = b'\0' * 3 + b'\n'

    def pack_data(self):
        return bytearray(self.data)

    def set_name(self, name):
        name = name.encode('utf-8')[:3] + b'\n'
        self.data.name = (c_char * 4).from_buffer_copy(name) 

def send_command(client_socket, player):
    data = player.pack_data()
    client_socket.sendall(data)
    print(f"Sent command: player_id={player.data.player_id}, state={player.data.state}, mv_dir={player.data.mv_dir}, atk_dir={player.data.atk_dir}, pos_x={player.data.pos_x}, pos_y={player.data.pos_y}, name={player.data.name.decode('utf-8')}")

def main():
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    try:
        client_socket.connect((server_address, server_port))
        print(f"Connected to {server_address}:{server_port}")

        player = Player(1)  # Create a player with ID 1
        player.data.name = b'ABC\n'  # Set the player's name (up to 4 characters)

        while True:
            print("Available commands:")
            print("move left : ml")
            print("move right : mr")
            print("move down : md")
            print("move up : mu")
            print("attack left : al")
            print("attack right : ar")
            print("attack down : ad")
            print("attack up : au")
            print("jump : j")
            print("crouch : c")
            print("disconnect : q")

            command = input("Enter a command: ")

            if command.lower() == 'q':
                break
            elif command.lower() == 'ml':
                player.data.mv_dir = 2
                send_command(client_socket, player)
            elif command.lower() == 'mr':
                player.data.mv_dir = 1
                send_command(client_socket, player)
            elif command.lower() == 'md':
                player.data.mv_dir = 0
                send_command(client_socket, player)
            elif command.lower() == 'mu':
                player.data.mv_dir = 3
                send_command(client_socket, player)
            elif command.lower() == 'al':
                player.data.atk_dir = 2
                send_command(client_socket, player)
            elif command.lower() == 'ar':
                player.data.atk_dir = 1
                send_command(client_socket, player)
            elif command.lower() == 'ad':
                player.data.atk_dir = 0
                send_command(client_socket, player)
            elif command.lower() == 'au':
                player.data.atk_dir = 3
                send_command(client_socket, player)
            elif command.lower() == 'j':
                player.data.state = 3
                send_command(client_socket, player)
            elif command.lower() == 'c':
                player.data.state = 2
                send_command(client_socket, player)
            else:
                print("Invalid command.")

    except ConnectionRefusedError:
        print(f"Connection refused by the server {server_address}:{server_port}")

    except Exception as e:
        print(f"An error occurred: {str(e)}")

    finally:
        client_socket.close()
        print("Disconnected from the server.")

if __name__ == '__main__':
    main()
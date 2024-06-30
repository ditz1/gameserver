import socket
import struct

# Server address and port
server_address = '192.168.1.42'
server_port = 8080
class Player:
    def __init__(self, player_id):
        self.player_id = player_id
        self.state = 0  # 0: alive, 1: dead, 2: crouch, 3: jump
        self.mv_dir = 0  # 0: down, 1: right, 2: left, 3: up
        self.atk_dir = 0  # 0: not attacking, 1: right, 2: left, 3: up
        self.pos_x = 0.0
        self.pos_y = 0.0
        self.name = b'ABC\n'  # Initialize with a default name

    def pack_data(self):
        # Pack data using struct
        s = struct.pack(
            'BBBBff',
            self.player_id,
            self.state,
            self.mv_dir,
            self.atk_dir,
            self.pos_x,
            self.pos_y
        )
        return s + self.name  # Append the name bytes directly

    def set_name(self, name):
        # Ensure the name is exactly 3 characters long, then add a newline
        self.name = name[:3].encode('ascii') + b'\n'

# In the main function:

def send_command(client_socket, player):
    data = player.pack_data()
    for byte in data:
        print(f'{hex(byte)}', end='|')
    print()
    client_socket.sendall(data)

def main():
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    try:
        client_socket.connect((server_address, server_port))
        print(f"Connected to {server_address}:{server_port}")

        player = Player(1)  # Create a player with ID 1
        player.set_name("dan")  # Set the player's name (up to 3 characters)

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
            command = command.strip()

            if command.lower() == 'q':
                break
            elif command.lower() == 'ml':
                player.mv_dir = 2
                send_command(client_socket, player)
            elif command.lower() == 'mr':
                player.mv_dir = 1
                send_command(client_socket, player)
            elif command.lower() == 'md':
                player.mv_dir = 0
                send_command(client_socket, player)
            elif command.lower() == 'mu':
                player.mv_dir = 3
                send_command(client_socket, player)
            elif command.lower() == 'al':
                player.atk_dir = 2
                send_command(client_socket, player)
            elif command.lower() == 'ar':
                player.atk_dir = 1
                send_command(client_socket, player)
            elif command.lower() == 'ad':
                player.atk_dir = 0
                send_command(client_socket, player)
            elif command.lower() == 'au':
                player.atk_dir = 3
                send_command(client_socket, player)
            elif command.lower() == 'j':
                player.state = 3
                send_command(client_socket, player)
            elif command.lower() == 'c':
                player.state = 2
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

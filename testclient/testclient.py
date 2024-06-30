import socket
import struct
import pygame

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

def send_command(client_socket, player):
    try:
        data = player.pack_data()
        client_socket.sendall(data)
        print("sent command")
    except Exception as e:
        print(f"Error: {e}")

def main():
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    
    pygame.init()
    screen = pygame.display.set_mode((800, 600))
    pygame.display.set_caption("Client")
    
    try:
        client_socket.connect((server_address, server_port))
        print(f"Connected to {server_address}:{server_port}")
        player = Player(1)  # Create a player with ID 1
        player.set_name("dan")  # Set the player's name (up to 3 characters)

        running = True
        while running:
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    running = False
                elif event.type == pygame.KEYUP:
                    if event.key == pygame.K_q:
                        running = False
                    elif event.key == pygame.K_a:
                        player.mv_dir = 2
                        send_command(client_socket, player)
                    elif event.key == pygame.K_d:
                        player.mv_dir = 1
                        send_command(client_socket, player)
                    elif event.key == pygame.K_s:
                        player.mv_dir = 0
                        send_command(client_socket, player)
                    elif event.key == pygame.K_w:
                        player.mv_dir = 3
                        send_command(client_socket, player)
                    elif event.key == pygame.K_LEFT:
                        player.atk_dir = 2
                        send_command(client_socket, player)#
                    elif event.key == pygame.K_RIGHT:
                        player.atk_dir = 1
                        send_command(client_socket, player)
                    elif event.key == pygame.K_DOWN:
                        player.atk_dir = 0
                        send_command(client_socket, player)
                    elif event.key == pygame.K_UP:
                        player.atk_dir = 3
                        send_command(client_socket, player)
                    elif event.key == pygame.K_SPACE:
                        player.state = 3
                        send_command(client_socket, player)
                    elif event.key == pygame.K_c:
                        player.state = 2
                        send_command(client_socket, player)

            screen.fill((255, 255, 255))
            font = pygame.font.Font(None, 36)
            text = font.render("Press 'Q' to quit", True, (0, 0, 0))
            screen.blit(text, (10, 10))
            pygame.display.flip()

    except ConnectionRefusedError:
        print(f"Connection refused by the server {server_address}:{server_port}")
    except Exception as e:
        print(f"An error occurred: {str(e)}")
    finally:
        client_socket.close()
        pygame.quit()
        print("Disconnected from the server.")

if __name__ == '__main__':
    main()
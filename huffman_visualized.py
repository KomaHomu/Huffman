import pygame
import huffman_demo as huffman

# Define colors
BLACK = (0, 0, 0)
WHITE = (255, 255, 255)
GREEN = (0, 255, 0)
GRAY = (200, 200, 200)
RED = (250, 0, 0)
ACTIVE_COLOR = (100, 100, 255)
INACTIVE_COLOR = (150, 150, 150)

# Initialize Pygame
pygame.init()

# Set up the window
width, height = pygame.display.Info().current_w, pygame.display.Info().current_h
# width, height = 1366, 768
margin = 270
window = pygame.display.set_mode((width, height))
pygame.display.set_caption("Huffman Tree Visualization")

# Scroll
screen = pygame.Surface((10000, 10000))
screen_x = 0
screen_y = 0
speed = 50
end_of_screen_x = 0
end_of_screen_y = 0
# Define the font
font = pygame.font.Font(None, 30)

# Text box variables
input_box = pygame.Rect(margin + width // 20, 20, width * 0.6, 32)
input_text = ""
input_active = False
fixed_string = "Hello, World!"
encoded_message = ""
decoded_message = ""

# Button variables
generate_button_rect = pygame.Rect(input_box.x - 320, input_box.y - 10, 180, 50)
y_button = generate_button_rect.y + generate_button_rect.height + 20
reset_button_rect = pygame.Rect(generate_button_rect.x , y_button, 100, 50)
prev_button_rect = pygame.Rect(reset_button_rect.x + reset_button_rect.width + margin // 10, y_button, 100, 50)
next_button_rect = pygame.Rect(prev_button_rect.x + prev_button_rect.width + margin // 10, y_button, 100, 50)
decode_button_rect = pygame.Rect(width // 2 - 90, height//10*8.75, 180, 50)
next_decode_button_rect = pygame.Rect(decode_button_rect.x + decode_button_rect.width + margin // 10,
                                      decode_button_rect.y, 100, 50)
decode_button_active = False

# Tree visualization variables
current_step = 0
construction_complete = False
tree_steps = []


def visualize_huffman_heap(nodes):
    global end_of_screen_x
    padding_for_child = 100

    x = width // 20
    # Calculate width by finding the maximum node in a line
    n = len(nodes)
    temp = nodes[-1]
    while temp.right is not None:
        temp = temp.right
        n += 2
    temp = nodes[0]
    while temp.left is not None:
        temp = temp.left
        n += 2

    # Adjust step size based on the number of nodes
    if n <= 10:
        step = width // (n + 1)
    else:
        step = width // 12

    for node in nodes:
        temp = node

        # Calc padding
        height_left = 1
        padding_left = 0

        while temp.left is not None:
            temp = temp.left
            padding_left += padding_for_child * 2 + 60
            height_left += 1

        height_right = 1
        padding_right = 0
        temp = node
        while temp.right is not None:
            temp = temp.right
            padding_right += padding_for_child * 2 + 60
            height_right += 1
        x += max(padding_left, padding_right)
        visualize_huffman_tree(node, x, reset_button_rect.y + reset_button_rect.height + 100, padding_for_child * max(height_left, height_right), 150)

        # Increment x position
        if node.right is not None:
            x += padding_right * 0.8
        x += step

    # Update the end of screen
    end_of_screen_x = x - width + 100


def visualize_huffman_tree(node, x, y, dx, dy, code=""):
    global end_of_screen_y
    if node is None:
        return

    # Plot the node
    radius = 35
    pygame.draw.circle(screen, GRAY, (x, y), radius)
    pygame.draw.circle(screen, BLACK, (x, y), radius, 2)

    # Draw the text on the node
    if node.char is not None:
        text_surface = font.render(f"{node.char} ({node.freq})", True, BLACK)
    else:
        text_surface = font.render(str(node.freq), True, BLACK)
    text_rect = text_surface.get_rect(center=(x, y))
    screen.blit(text_surface, text_rect)

    # Draw the code on the node if it's a leaf node
    if node.left is None and node.right is None:
        code_surface = font.render(code, True, BLACK)
        code_rect = code_surface.get_rect(center=(x, y + radius + 20))
        screen.blit(code_surface, code_rect)
        # Update the end of screen
        if y + radius + 20 > end_of_screen_y:
            end_of_screen_y = y + radius + 200 - height
        if end_of_screen_y < 0:
            end_of_screen_y = 0
    # Plot the left child
    if node.left is not None:
        x_left = x - dx
        y_left = y + dy
        pygame.draw.line(screen, BLACK, (x, y + radius), (x_left, y_left - radius), 2)
        code_text = font.render("0", True, BLACK)
        code_rect = code_text.get_rect(center=(x_left + dx / 2, y_left - radius))
        screen.blit(code_text, code_rect)

        visualize_huffman_tree(node.left, x_left, y_left, dx / 2, dy, code + "0")

    # Plot the right child
    if node.right is not None:
        x_right = x + dx
        y_right = y + dy
        pygame.draw.line(screen, BLACK, (x, y + radius), (x_right, y_right - radius), 2)
        code_text = font.render("1", True, BLACK)
        code_rect = code_text.get_rect(center=(x_right - dx / 2, y_right - radius))
        screen.blit(code_text, code_rect)
        visualize_huffman_tree(node.right, x_right, y_right, dx / 2, dy, code + "1")


def visualize_decoding_huffman_heap(curr_node, nodes):
    global end_of_screen_x
    padding_for_child = 100

    x = width // 20
    # Calculate width by finding the maximum node in a line
    n = len(nodes)
    temp = nodes[-1]
    while temp.right is not None:
        temp = temp.right
        n += 2
    temp = nodes[0]
    while temp.left is not None:
        temp = temp.left
        n += 2

    # Adjust step size based on the number of nodes
    if n <= 10:
        step = width // (n + 1)
    else:
        step = width // 12

    for node in nodes:
        temp = node

        # Calc padding
        height_left = 1
        padding_left = 0

        while temp.left is not None:
            temp = temp.left
            padding_left += padding_for_child * 2 + 60
            height_left += 1

        height_right = 1
        padding_right = 0
        temp = node
        while temp.right is not None:
            temp = temp.right
            padding_right += padding_for_child * 2 + 60
            height_right += 1
        x += max(padding_left, padding_right)
        visualize_decoding_huffman_tree(curr_node, node, x, 200, padding_for_child * max(height_left, height_right),
                                        150)

        # Increment x position
        if node.right is not None:
            x += padding_right * 0.8
        x += step

    # Update the end of screen
    end_of_screen_x = x - width + 100


def visualize_decoding_huffman_tree(decoded_node, node, x, y, dx, dy, code=""):
    global end_of_screen_y
    if node is None or decoded_node is None:
        return

    # Plot the node
    radius = 35
    if node == decoded_node:
        pygame.draw.circle(screen, RED, (x, y), radius)
    else:
        pygame.draw.circle(screen, GRAY, (x, y), radius)
    pygame.draw.circle(screen, BLACK, (x, y), radius, 2)

    # Draw the text on the node
    if node.char is not None:
        text_surface = font.render(f"{node.char} ({node.freq})", True, BLACK)
    else:
        text_surface = font.render(str(node.freq), True, BLACK)
    text_rect = text_surface.get_rect(center=(x, y))
    screen.blit(text_surface, text_rect)

    # Draw the code on the node if it's a leaf node
    if node.left is None and node.right is None:
        code_surface = font.render(code, True, BLACK)
        code_rect = code_surface.get_rect(center=(x, y + radius + 20))
        screen.blit(code_surface, code_rect)
        # Update the end of screen
        if y + radius + 20 > end_of_screen_y:
            end_of_screen_y = y + radius + 200 - height
        if end_of_screen_y < 0:
            end_of_screen_y = 0
    # Plot the left child
    if node.left is not None:
        x_left = x - dx
        y_left = y + dy
        pygame.draw.line(screen, BLACK, (x, y + radius), (x_left, y_left - radius), 2)
        code_text = font.render("0", True, BLACK)
        code_rect = code_text.get_rect(center=(x_left + dx / 2, y_left - radius))
        screen.blit(code_text, code_rect)

        visualize_decoding_huffman_tree(decoded_node, node.left, x_left, y_left, dx / 2, dy, code + "0")

    # Plot the right child
    if node.right is not None:
        x_right = x + dx
        y_right = y + dy
        pygame.draw.line(screen, BLACK, (x, y + radius), (x_right, y_right - radius), 2)
        code_text = font.render("1", True, BLACK)
        code_rect = code_text.get_rect(center=(x_right - dx / 2, y_right - radius))
        screen.blit(code_text, code_rect)

        visualize_decoding_huffman_tree(decoded_node, node.right, x_right, y_right, dx / 2, dy, code + "1")


def build_tree_with_steps(data):
    list_char = huffman.calc_freq(data)
    heap = []
    steps = []
    for char, freq in list_char.items():
        heap.append(huffman.Node(char, freq))

    steps.append(heap.copy())

    while len(heap) > 1:
        heap.sort()
        if heap != steps[-1]:
            steps.append(heap.copy())
        left = heap.pop(0)
        right = heap.pop(0)
        combined_node = huffman.Node(left=left, right=right)
        heap.append(combined_node)
        steps.append(heap.copy())

    return steps


def main():
    global current_step
    global construction_complete
    global tree_steps
    global fixed_string
    global input_text
    global input_active
    global encoded_message
    global decoded_message
    global screen_x
    global screen_y
    global screen
    global speed

    decode_button_active = False
    decode_step = -1

    running = True
    active = False
    while running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False

            if event.type == pygame.MOUSEBUTTONDOWN and event.button == 1:
                mouse_pos = pygame.mouse.get_pos()

                # Check if the input box is clicked
                if input_box.collidepoint(mouse_pos):
                    input_active = True
                else:
                    input_active = False

                # Check if the reset button is clicked
                if reset_button_rect.collidepoint(mouse_pos):
                    current_step = 0
                    construction_complete = False
                    encoded_message = ""
                    tree_steps = []
                    input_text = ""
                    active = False
                    decoded_message = ""
                    decode_button_active = False
                    decode_step = -1

                # Check if the previous button is clicked
                if prev_button_rect.collidepoint(mouse_pos) and current_step > 0 and construction_complete:
                    current_step -= 1

                # Check if the next button is clicked
                if next_button_rect.collidepoint(mouse_pos) and current_step < len(
                        tree_steps) - 1 and construction_complete:
                    current_step += 1

                # Check if the next decode button is clicked
                if next_decode_button_rect.collidepoint(mouse_pos) and decode_step < len(
                        encoded_message) - 1 and decode_button_active:
                    decode_step += 1
                    active = True

                # Check if the decode button is clicked
                if decode_button_rect.collidepoint(mouse_pos) and construction_complete and current_step == len(
                        tree_steps) - 1 and input_text != decoded_message and not decode_button_active:
                    decode_button_active = True
                    code = encoded_message
                    current = tree_steps[current_step][0]

                # Check if the generate button is clicked
                if generate_button_rect.collidepoint(mouse_pos):
                    input_text = fixed_string  # Set the input text to the fixed string
                    encoded_message = huffman.encode(input_text)[3]
                    tree_steps = build_tree_with_steps(input_text)
                    construction_complete = True
                    active = False
                    decoded_message = ""
                    decode_button_active = False
                    decode_step = -1

            if event.type == pygame.KEYDOWN:
                if input_active:
                    if event.key == pygame.K_RETURN and input_text != "":
                        encoded_message = huffman.encode(input_text)[3]
                        tree_steps = build_tree_with_steps(input_text)
                        construction_complete = True
                        input_active = False
                        current_step = 0
                        decoded_message = ""
                        decode_button_active = False
                        decode_step = -1
                    elif event.key == pygame.K_BACKSPACE and len(input_text) > 0:
                        input_text = input_text[:-1]
                    elif len(input_text) < 104:
                        input_text += event.unicode
                else:
                    # camera movement
                    if event.key == pygame.K_LEFT and screen_x < 0:
                        window.fill(WHITE)
                        screen_x += speed
                    if event.key == pygame.K_RIGHT and screen_x > - end_of_screen_x:
                        window.fill(WHITE)
                        screen_x -= speed
                    if event.key == pygame.K_UP and screen_y < 0:
                        window.fill(WHITE)
                        screen_y += speed
                    if event.key == pygame.K_DOWN and screen_y > - end_of_screen_y:
                        window.fill(WHITE)
                        screen_y -= speed
                    if event.key == pygame.K_EQUALS:
                        speed += 10
                    if event.key == pygame.K_MINUS and speed > 10:
                        speed -= 10
                    if event.key == pygame.K_TAB:
                        screen_x = 0
                        screen_y = 0
                    if event.key == pygame.K_SPACE:
                        screen_x = - end_of_screen_x
                        screen_y = - end_of_screen_y

        if decode_button_active:
            c = code[decode_step]

            if active:
                if c == '0':
                    current = current.left
                elif c == '1':
                    current = current.right
                current.set_freq(current.freq - 1)
                active = False

            screen.fill(WHITE)
            visualize_decoding_huffman_heap(current, tree_steps[current_step])

            encoded_text = font.render("Encoding Message: " + encoded_message[decode_step + 1:], True, BLACK)
            screen.blit(encoded_text,
                        (reset_button_rect.x, reset_button_rect.y + reset_button_rect.height + 20))

            window.blit(screen, (screen_x, screen_y))
            if current.char is not None:
                decoded_message += current.char
                current = tree_steps[current_step][0]

        if not decode_button_active:
            # Draw screen
            screen.fill(WHITE)
            if current_step < len(tree_steps):
                # Construct the Huffman tree step-by-step
                visualize_huffman_heap(tree_steps[current_step])
                # Draw the encoded message
                encoded_text = font.render("Encoded Message: " + encoded_message, True, BLACK)
                screen.blit(encoded_text,
                            (reset_button_rect.x, reset_button_rect.y + reset_button_rect.height + 20))
            window.blit(screen, (screen_x, screen_y))

        # Draw the input box
        if input_active:
            pygame.draw.rect(window, ACTIVE_COLOR, input_box)
        else:
            pygame.draw.rect(window, INACTIVE_COLOR, input_box)

        # Render the input text on the screen
        input_surface = font.render(input_text, True, BLACK)
        window.blit(input_surface, (input_box.x + 5, input_box.y + 5))

        # Draw the Generate String box
        pygame.draw.rect(window, GRAY, generate_button_rect)
        generate_button_text = font.render("Generate String", True, BLACK)
        generate_button_text_rect = generate_button_text.get_rect(center=generate_button_rect.center)
        window.blit(generate_button_text, generate_button_text_rect)

        # Draw the "Input box:" text
        input_text_surface = font.render("Input box:", True, BLACK)
        window.blit(input_text_surface, (input_box.x - 120, input_box.y + 5))

        # Draw the Reset button
        pygame.draw.rect(window, GRAY, reset_button_rect)
        reset_button_text = font.render("Reset", True, BLACK)
        reset_button_text_rect = reset_button_text.get_rect(center=reset_button_rect.center)
        window.blit(reset_button_text, reset_button_text_rect)

        # Draw the Previous button
        pygame.draw.rect(window, GRAY, prev_button_rect)
        prev_button_text = font.render("Previous", True, BLACK)
        prev_button_text_rect = prev_button_text.get_rect()
        prev_button_text_rect.center = prev_button_rect.center
        window.blit(prev_button_text, prev_button_text_rect)

        # Draw the Next button
        pygame.draw.rect(window, GRAY if current_step < len(tree_steps) - 1 and construction_complete else RED,
                         next_button_rect)
        next_button_text = font.render("Next", True, BLACK)
        next_button_text_rect = next_button_text.get_rect()
        next_button_text_rect.center = next_button_rect.center
        window.blit(next_button_text, next_button_text_rect)

        # Draw the Decode button
        if construction_complete and current_step == len(tree_steps) - 1:
            pygame.draw.rect(window, GRAY, decode_button_rect)
            if input_text != decoded_message:
                decode_button_text = font.render("Decode", True, BLACK)
            else:
                decode_button_text = font.render("Decoded", True, BLACK)
            decode_button_text_rect = decode_button_text.get_rect()
            decode_button_text_rect.center = decode_button_rect.center
            window.blit(decode_button_text, decode_button_text_rect)

            # Draw the "Decoded Message:" text
            decoded_text = font.render("Decoded Message: " + decoded_message, True, BLACK)
            window.blit(decoded_text,
                        (decode_button_rect.x, decode_button_rect.y - 30))

        if decode_button_active:
            pygame.draw.rect(window, GRAY, next_decode_button_rect)
            next_decode_button_text = font.render("Next", True, BLACK)
            next_decode_button_text_rect = next_decode_button_text.get_rect()
            next_decode_button_text_rect.center = next_decode_button_rect.center
            window.blit(next_decode_button_text, next_decode_button_text_rect)

        if decode_step >= len(encoded_message) - 1:
            decode_button_active = False

        pygame.display.flip()

    pygame.quit()


if __name__ == '__main__':
    main()

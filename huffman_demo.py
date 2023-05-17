class Node:
    def __init__(self, char: str = None, freq: int = None, left=None, right=None, father=None):
        if left is not None or right is not None:
            self.char = None
            self.freq = (left.freq if left is not None else 0) + (right.freq if right is not None else 0)
            self.left = left
            self.right = right
            self.father = father
            if left is not None:
                left.father = self
            if right is not None:
                right.father = self
        else:
            self.char = char
            self.freq = freq
            self.left = None
            self.right = None
            self.father = father

    def __lt__(self, other):
        return self.freq < other.freq

    def __eq__(self, other):
        return self.freq == other.freq

    def __gt__(self, other):
        return self.freq > other.freq

    def __le__(self, other):
        return self.freq <= other.freq

    def __ge__(self, other):
        return self.freq >= other.freq

    def __str__(self):
        return f'Node[({self.char}), left: [{self.left}], right: [{self.right}]]'
    def __repr__(self):
        return str(self)

    def to_table(self):
        node = 0
        tree = ''
        char = []
        if self.char is not None:
            return 1, tree, [self.char]
        else:
            node_l, tree_l, char_l = self.left.to_table() if self.left is not None else (0, '', [])
            node_r, tree_r, char_r = self.right.to_table() if self.right is not None else (0, '', [])
            char = char_l + char_r
            node = node_l + node_r + 1
            if self.left is not None:
                tree += '0' + tree_l
            if self.right is not None:
                tree += '1' + tree_r
            return [node, tree, char]
    def leafs(self):
        if self.left is None and self.right is None:
            return [self]
        else:
            return (self.left.leafs() if self.left is not None else []) + (self.right.leafs() if self.right is not None else [])
        
def calc_freq(data):
    list_char = {}
    for char in data:
        if char in list_char:
            list_char[char] += 1
        else:
            list_char[char] = 1
    return list_char

def build_tree(data):
    list_char = calc_freq(data)
    heap = []
    for char, freq in list_char.items():
        heap.append(Node(char, freq))
    while len(heap) > 1:
        heap.sort()
        left = heap.pop(0)
        right = heap.pop(0)
        heap.append(Node(left=left, right=right))
    if heap[0].char is not None:
        return Node(left=heap[0])
    return heap[0]

def tree_to_dict(root):
    huffman_dict = {}
    def traverse(node, code):
        if node is None:
            return

        if node.char is not None:
            huffman_dict[node.char] = code
            return

        traverse(node.left, code + '0')
        traverse(node.right, code + '1')

    traverse(root, '')
    return huffman_dict

def get_code(huffman_dict, char):
    for c, code in huffman_dict.items():
        if c == char:
            return code
    return None

def table_to_tree(code):
    node = code[0] - 1# Lấy số nút trừ root
    preorder=code[1] # Lấy preorder của cây
    # Xây cây
    root = Node()
    current = root 
    for i in range(len(preorder)):
        if preorder[i] == '0':
            current.left = Node()
            current.left.father = current
            current = current.left
        elif preorder[i] == '1':
            current = current.father
            while current.right is not None:
                current = current.father
            current.right = Node()
            current.right.father = current
            current = current.right
    # Chèn kí tự cho nút lá
    leafs = root.leafs()
    for i in range(len(leafs)):
        leafs[i].char = code[2][i]
    return root, code[-1]

def encode(data):
    root = build_tree(data)
    huffman_dict = tree_to_dict(root)
    res = ''
    for char in data:
        res += huffman_dict[char]
    res = root.to_table() + [res]
    return res

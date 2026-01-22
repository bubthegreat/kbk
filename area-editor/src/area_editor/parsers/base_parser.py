"""
Base parser class with common parsing utilities.
"""
from typing import TextIO, Optional


class BaseParser:
    """Base class for parsing .are file sections."""
    
    def __init__(self, file_handle: TextIO):
        """Initialize the parser with a file handle."""
        self.file = file_handle
        self.current_line = ""
        self.line_number = 0
    
    def read_line(self) -> str:
        """Read the next line from the file."""
        line = self.file.readline()
        self.line_number += 1
        self.current_line = line.rstrip('\n\r')
        return self.current_line
    
    def read_word(self) -> str:
        """Read a single word (non-blank characters terminated by blank)."""
        # Skip leading whitespace
        while True:
            if not self.current_line or self.current_line.isspace():
                self.read_line()
            else:
                break
        
        # Extract word
        self.current_line = self.current_line.lstrip()
        if not self.current_line:
            return ""
        
        parts = self.current_line.split(None, 1)
        word = parts[0] if parts else ""
        self.current_line = parts[1] if len(parts) > 1 else ""
        return word
    
    def read_string(self) -> str:
        """Read a tilde-terminated string (may span multiple lines)."""
        result = []
        
        while True:
            if not self.current_line:
                self.read_line()
            
            # Check for tilde
            tilde_pos = self.current_line.find('~')
            if tilde_pos >= 0:
                result.append(self.current_line[:tilde_pos])
                self.current_line = self.current_line[tilde_pos + 1:]
                break
            else:
                result.append(self.current_line)
                result.append('\n')
                self.current_line = ""
        
        return ''.join(result)
    
    def read_number(self) -> int:
        """Read a number.

        Handles:
        - Regular integers: "123"
        - Bit flags with |: "1|2|4"
        - Letter-coded flags: "AB" or "ACDEFHIJKN"
        """
        word = self.read_word()
        if not word:
            return 0

        # Handle | notation for bit flags
        if '|' in word:
            total = 0
            for part in word.split('|'):
                total += int(part) if part else 0
            return total

        # Try to parse as integer
        try:
            return int(word)
        except ValueError:
            # If it fails, it's likely a letter-coded flag
            # Convert letter codes to bit values (A=1, B=2, C=4, D=8, etc.)
            total = 0
            for char in word.upper():
                if char.isalpha():
                    # A=0, B=1, C=2, etc. -> bit value = 2^position
                    position = ord(char) - ord('A')
                    total |= (1 << position)
            return total
    
    def read_to_eol(self) -> str:
        """Read to end of line."""
        result = self.current_line
        self.current_line = ""
        return result.strip()
    
    def peek_word(self) -> str:
        """Peek at the next word without consuming it."""
        saved_line = self.current_line
        saved_pos = self.file.tell()
        saved_line_num = self.line_number
        
        word = self.read_word()
        
        self.file.seek(saved_pos)
        self.current_line = saved_line
        self.line_number = saved_line_num
        
        return word
    
    def skip_whitespace(self):
        """Skip whitespace and empty lines."""
        while self.current_line.isspace() or not self.current_line:
            if not self.read_line():
                break


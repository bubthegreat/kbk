"""
Configuration manager for the Area Editor.

Handles loading and saving user preferences and UI layout settings.
"""
import json
from pathlib import Path
from typing import Dict, Any, Optional


class Config:
    """Manages application configuration."""

    def __init__(self):
        """Initialize the configuration manager."""
        self.config_dir = Path.home() / ".kbk_area_editor"
        self.config_file = self.config_dir / "config.json"
        self.config: Dict[str, Any] = {}
        self._load()

    def _load(self):
        """Load configuration from file."""
        if self.config_file.exists():
            try:
                with open(self.config_file, 'r') as f:
                    self.config = json.load(f)
            except (json.JSONDecodeError, IOError) as e:
                print(f"Warning: Could not load config file: {e}")
                self.config = {}
        else:
            # Create default configuration
            self.config = self._get_default_config()

    def _get_default_config(self) -> Dict[str, Any]:
        """Get default configuration values."""
        return {
            "ui": {
                "left_sidebar_width": 300,
                "right_sidebar_width": 980,
                "window_width": 1600,
                "window_height": 900
            },
            "recent_files": []
        }

    def save(self):
        """Save configuration to file."""
        # Create config directory if it doesn't exist
        self.config_dir.mkdir(parents=True, exist_ok=True)
        
        try:
            with open(self.config_file, 'w') as f:
                json.dump(self.config, indent=2, fp=f)
        except IOError as e:
            print(f"Warning: Could not save config file: {e}")

    def get(self, key: str, default: Any = None) -> Any:
        """Get a configuration value.
        
        Args:
            key: Dot-separated key path (e.g., "ui.left_sidebar_width")
            default: Default value if key doesn't exist
            
        Returns:
            The configuration value or default
        """
        keys = key.split('.')
        value = self.config
        
        for k in keys:
            if isinstance(value, dict) and k in value:
                value = value[k]
            else:
                return default
        
        return value

    def set(self, key: str, value: Any):
        """Set a configuration value.
        
        Args:
            key: Dot-separated key path (e.g., "ui.left_sidebar_width")
            value: Value to set
        """
        keys = key.split('.')
        config = self.config
        
        # Navigate to the parent dictionary
        for k in keys[:-1]:
            if k not in config:
                config[k] = {}
            config = config[k]
        
        # Set the value
        config[keys[-1]] = value

    def get_ui_width(self, panel: str) -> int:
        """Get the width of a UI panel.
        
        Args:
            panel: Panel name ('left_sidebar' or 'right_sidebar')
            
        Returns:
            Width in pixels
        """
        return self.get(f"ui.{panel}_width", self._get_default_config()["ui"][f"{panel}_width"])

    def set_ui_width(self, panel: str, width: int):
        """Set the width of a UI panel.
        
        Args:
            panel: Panel name ('left_sidebar' or 'right_sidebar')
            width: Width in pixels
        """
        self.set(f"ui.{panel}_width", width)

    def get_window_size(self) -> tuple[int, int]:
        """Get the window size.
        
        Returns:
            Tuple of (width, height) in pixels
        """
        defaults = self._get_default_config()["ui"]
        width = self.get("ui.window_width", defaults["window_width"])
        height = self.get("ui.window_height", defaults["window_height"])
        return (width, height)

    def set_window_size(self, width: int, height: int):
        """Set the window size.
        
        Args:
            width: Width in pixels
            height: Height in pixels
        """
        self.set("ui.window_width", width)
        self.set("ui.window_height", height)


# Global configuration instance
config = Config()


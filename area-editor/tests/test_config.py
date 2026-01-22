"""
Tests for the configuration system.
"""
import pytest
import json
import tempfile
from pathlib import Path
from area_editor.config import Config


def test_config_default_values():
    """Test that default configuration values are set correctly."""
    # Create a temporary config with a non-existent file
    with tempfile.TemporaryDirectory() as tmpdir:
        config = Config()
        config.config_dir = Path(tmpdir)
        config.config_file = config.config_dir / "config.json"
        config._load()
        
        # Check default values
        assert config.get("ui.left_sidebar_width") == 300
        assert config.get("ui.right_sidebar_width") == 980
        assert config.get("ui.window_width") == 1600
        assert config.get("ui.window_height") == 900


def test_config_save_and_load():
    """Test that configuration can be saved and loaded."""
    with tempfile.TemporaryDirectory() as tmpdir:
        # Create config and set some values
        config1 = Config()
        config1.config_dir = Path(tmpdir)
        config1.config_file = config1.config_dir / "config.json"
        
        config1.set("ui.left_sidebar_width", 400)
        config1.set("ui.right_sidebar_width", 1000)
        config1.save()
        
        # Load config in a new instance
        config2 = Config()
        config2.config_dir = Path(tmpdir)
        config2.config_file = config2.config_dir / "config.json"
        config2._load()
        
        # Verify values were persisted
        assert config2.get("ui.left_sidebar_width") == 400
        assert config2.get("ui.right_sidebar_width") == 1000


def test_config_get_set_ui_width():
    """Test the convenience methods for UI width."""
    with tempfile.TemporaryDirectory() as tmpdir:
        config = Config()
        config.config_dir = Path(tmpdir)
        config.config_file = config.config_dir / "config.json"
        config._load()
        
        # Set and get left sidebar width
        config.set_ui_width("left_sidebar", 350)
        assert config.get_ui_width("left_sidebar") == 350
        
        # Set and get right sidebar width
        config.set_ui_width("right_sidebar", 1100)
        assert config.get_ui_width("right_sidebar") == 1100


def test_config_get_set_window_size():
    """Test the convenience methods for window size."""
    with tempfile.TemporaryDirectory() as tmpdir:
        config = Config()
        config.config_dir = Path(tmpdir)
        config.config_file = config.config_dir / "config.json"
        config._load()
        
        # Set and get window size
        config.set_window_size(1920, 1080)
        width, height = config.get_window_size()
        assert width == 1920
        assert height == 1080


def test_config_nested_keys():
    """Test that nested keys work correctly."""
    with tempfile.TemporaryDirectory() as tmpdir:
        config = Config()
        config.config_dir = Path(tmpdir)
        config.config_file = config.config_dir / "config.json"
        config._load()
        
        # Set nested values
        config.set("editor.font.size", 14)
        config.set("editor.font.family", "monospace")
        
        # Get nested values
        assert config.get("editor.font.size") == 14
        assert config.get("editor.font.family") == "monospace"
        
        # Get with default
        assert config.get("editor.font.color", "#ffffff") == "#ffffff"


"""
Shop data model.
"""
from dataclasses import dataclass, field
from typing import List


@dataclass
class Shop:
    """Represents a MUD shop."""
    
    keeper: int = 0  # Vnum of the shopkeeper mobile
    buy_types: List[int] = field(default_factory=list)  # Item types the shop will buy (up to 5)
    profit_buy: int = 100  # Markup percentage for buying (100 = nominal)
    profit_sell: int = 100  # Markdown percentage for selling (100 = nominal)
    open_hour: int = 0  # Hour shop opens (0-23)
    close_hour: int = 23  # Hour shop closes (0-23)


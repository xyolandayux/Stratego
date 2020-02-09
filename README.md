# Stratego

An implementation of the popular board game Stratego programmed in C++, utilizing conceots like the observer design pattern and MVC model.
Both a graphical (on XQuartz - X11) and text (on stdout) representation of the game board is displayed after each turn.

Setup:
This is a two players (can easily be extended to more players if needed) game consists of a 8x8 board and 8 links each.
A link is either a Data or a Virus, and has a strength between 1 - 4 inclusive.

Goal of the game:
To download 4 datas or make your opponent download 4 viruses.

A turn can consist of using an ability, or moving a link. 
A link can move horizontally and vertically, but not diagonally. It cannot move to on top of another link of the same player.
If a link is moved on top of an opponents link, their strengths are revealed and the winner downloads the link of the loser.
If a link is moved on top of the opponent's server port, it gets downloaded by them.
If a link is moved off the edge on the opposite side, it gets downloaded by the owner.


Abilities includes:
  Link Boost (L) : allows one link to move 2 spaces 
  Firewall (F) : placed on a cell in the grid, 
                 opponents links are revealed when passing this cell and if it's a virus, 
                 then it gets downlaoded by their owner
  Trade (T) : trade a link with the opponent 
  Skip (S) : skip the turn without moving
  Battle (B) : instant battle regardless of the links location
  Download (D) : immediately downloads an opponents link
  Scan (S): reveals the type and strength of any link
  Polarize (P) : changes the type of a specific link 
  
Each player can choose 5 abilites at the start of the game, max 2 of each type and each ability can only be used once in the game. 

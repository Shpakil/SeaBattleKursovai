#pragma once
#include "Player.h"
#include <utility>
#include <string>
#include <vector>
#include <queue>

enum class AIMode {
    Search,     // ����� ������ ��������
    Hunt        // ����� ��������� �������� �������
};

class ComputerPlayer : public Player
{
private:
    std::vector<std::pair<int, int>> availableMoves;

    // ������ ���������
    AIMode currentMode;
    std::pair<int, int> firstHit;          // ������ ��������� �� �������
    std::pair<int, int> lastHit;           // ��������� ��������� �� �������
    std::queue<std::pair<int, int>> huntMoves; // ������� ����� ��� ���������
    ShipOrientation suspectedOrientation;  // �������������� ���������� �������
    bool orientationConfirmed;             // ���������� ������������

public:
    ComputerPlayer(const std::string& n = "Computer");
    void updateAIMode(int lastRow, int lastCol, bool hit, bool sink = false);
    void reset() override;
    void placeShips() override;
    std::pair<int, int> makeMove() override;
    void removeMoveFromAvailable(int row, int col);

    // ����� �����: ������������� � ������ ����������
    void syncAvailableMovesWithBoard(const Board& opponentBoard);

private:
    bool tryPlaceShip(int startRow, int startCol, int size, ShipOrientation orientation);
    void generateAvailableMoves();
    void clearHuntState();

    // ������ ��� ���������������� ��������
    std::pair<int, int> makeSearchMove();  // ��������� �����
    std::pair<int, int> makeHuntMove();    // ��������� �������

    void generateHuntMoves(int hitRow, int hitCol);
    void generateTargetedHuntMoves();      // ������� ��������� ����� ����������� ����������
    bool isValidHuntTarget(int row, int col) const;
};



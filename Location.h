#pragma once

/*
    struct représentant une position sur la carte
*/
struct Location
{
    int row, col;

    Location()
    {
        row = col = 0;
    };

    Location(int r, int c)
    {
        row = r;
        col = c;
    };

    // Opérateur de comparaison ==
    bool operator==(const Location& other) const
    {
        return row == other.row && col == other.col;
    }

    // Opérateur de comparaison != (inverse de ==)
    bool operator!=(const Location& other) const
    {
        return !(*this == other);
    }

    // Opérateur d'addition entre deux Location
    Location operator+(const Location& loc2) const
    {
        return Location(row + loc2.row, col + loc2.col);
    }
};

#pragma once

/*
    struct for representing locations in the grid.
*/
struct Location
{
    int row, col;

    Location()
    {
        row = col = 0;
    }

    Location(int r, int c)
    {
        row = r;
        col = c;
    }

    Location(Location& l){
        row = l.row;
        col = l.col;
    }

    bool operator==(const Location& other) const
    {
        return row == other.row && col == other.col;
    }

    bool operator!=(const Location& other) const
    {
        return !(*this == other);
    }

    Location operator+(const Location& loc2) const
    {
        return Location(row + loc2.row, col + loc2.col);
    }
};

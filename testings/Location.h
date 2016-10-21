#ifndef LOCATION_H_
#define LOCATION_H_

/*
    struct for representing locations in the grid.
*/
struct Location {
    int row, col;

    Location() {
        row = col = 0;
    };

    Location(int r, int c) {
        row = r;
        col = c;
    };
    
    bool operator <(const Location& rhs) const {return this->row < rhs.row;}
    bool operator ==(const Location& rhs) const {return (this->row == rhs.row && this->col == rhs.col);}
    bool operator !=(const Location& rhs) const {return (this->row != rhs.row || this->col != rhs.col);}
};

#endif //LOCATION_H_

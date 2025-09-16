#ifndef CELL_H  
#define CELL_H  
#include "ship.h"  
class Ship; // Forward declaration of Ship class  

class Cell  
{  
public:  
   enum State {  
       Empty,      // Пустая клетка  
       ShipState,  // Renamed to avoid conflict with Ship class  
       Hit,        // Попадание  
       Miss,       // Промах  
       Destroyed   // Уничтоженный корабль  
   };  

   Cell();  

   State state() const;  
   void setState(State state);  

   bool hasShip() const;  
   Ship* ship() const;  
   void setShip(Ship* ship);  

private:  
   State m_state;  
   Ship* m_ship;  
};  

#endif // CELL_H

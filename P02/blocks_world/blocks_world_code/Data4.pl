% a blocks world
block(b1).
block(b2).
block(b3).
block(b4).
block(b5).
block(b6).
place(6).
place(4).
place(5).
place(1).
place(2).
place(3).

start([clear(b1),clear(b6),clear(2),clear(b4),clear(4),clear(6),on(b1,1),on(b6,b2),on(b2,b3),on(b3,3),on(b4,b5),on(b5,5)]).

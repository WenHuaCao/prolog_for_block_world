% a blocks world
block(b1).
block(b2).
block(b3).
block(b4).
block(b5).
block(b6).
block(b7).
block(b8).
place(7).
place(8).
place(6).
place(4).
place(5).
place(1).
place(2).
place(3).


start([clear(b1),clear(b6),clear(b4),clear(b8),clear(2),clear(4),clear(7),clear(8),on(b1,1),on(b6,b2),on(b2,b3),on(b3,3),on(b4,b5),on(b5,5),on(b8,b7),on(b7,6)]).

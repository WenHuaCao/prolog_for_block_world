
% a blocks world
block(b1).
block(b2).
block(b3).
block(b4).
block(b5).
place(4).
place(5).
place(1).
place(2).
place(3).


start([on(b1,b5),on(b5,b2),on(b2,1),on(b3,b4),on(b4,2),clear(b1),clear(b3),clear(3),clear(4),clear(5)]).

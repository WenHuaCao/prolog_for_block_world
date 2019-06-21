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
can( move(Block,From,To), [clear(Block), on(Block,From), clear(To)] ) :-
    block(Block),
    object(To),
    To\==Block,
    object(From),
    From\==To,
    Block\==From.
adds(move(X,From,To),[on(X,To),clear(From)]).
deletes( move(X,From,To), [on(X,From),clear(To)] ).
object(X):-
    place(X) ; block(X).

:- op(300,xfy,->>).
succ(Goals ->> _, NewGoals ->> Action,1) :-
    member(Goal,Goals),
    achieves(Action,Goal),
    can(Action,_),
    preserves(Action,Goals),
    regress(Goals,Action,NewGoals).
goal(Goals ->> _) :-
    start(State),
    satisfied(State,Goals).
h(Goals ->> _,H):-
    start(State),
    %% write(Goals),
    mycheck(Goals,State,H).
    %% write(H).
mycheck(A,B,K):-
%% write(checkone),nl,
((setof(X,(not_on_same_pos(X,A,B)),R))->length(R,K1);(K1 = 0,R = []) ),(setof(D,(member(D,R),under(A,E,D),under(B,E,D)),R2)->length(R2,K2);K2 = 0),K is K1 + K2.
not_on_same_pos(X,A,B):-
member(on(X,Y),A),
place(Y),
member(on(X,Z),B),
Z \== Y.
not_on_same_pos(X,A,B):-
member(on(X,Y),A),
block(Y),
member(on(X,Z),B),
(Z\==Y;not_on_same_pos(Y,A,B)).

under(A,Y,X):-
    member(on(X,Y),A),
    block(Y).
under(A,Y,X):-
    member(on(X,Z),A),
    block(Z),
    under(A,Y,Z).
satisfied(State,Goals) :-
    delete_all(Goals,State,[]).

select(_,Goals,Goal) :-
    member(Goal,Goals).

achieves(Action,Goal) :-
    adds(Action,Goals),
    member(Goal,Goals).

preserves(Action,Goals) :-
    deletes(Action,Relations),
    not((member(Goal,Relations),
    member(Goal,Goals))).

regress(Goals,Action,RegressGoals) :-
    adds(Action,NewRelations),
    delete_all(Goals,NewRelations,RestGoals),
    can(Action,Condition),
    addnew(Condition,RestGoals,RegressGoals).

addnew([],L,L).
addnew([Goal|_],Goals,_) :-
    impossible(Goal,Goals),!,fail.

addnew([X|L1],L2,L3) :-
    member(X,L2),!,
    addnew(L1,L2,L3).

addnew([X|L1],L2,[X|L3]) :-
    addnew(L1,L2,L3).

delete_all([],_,[]).
delete_all([X|L1],L2,Diff) :-
    member(X,L2),!,
    delete_all(L1,L2,Diff).
delete_all([X|L1],L2,[X|Diff]) :-
    delete_all(L1,L2,Diff).

impossible(on(X,X),_).
impossible(on(X,Y),Goals) :-
    member(clear(Y),Goals);
    member(on(X,Y1),Goals),Y1\==Y;
    member(on(X1,Y),Goals),X1\==X.
impossible(clear(X),Goals) :-
    member(on(_,X),Goals).
bestfirst(Start,Solution) :-
    expand([],l(Start,0/0),99999,_,yes,Solution).
expand(P,l(N,_),_,_,yes,[N|P]) :-
    goal(N).
expand(P,l(N,F/G),Bound,Tree1,Solved,Sol) :-
    F=<Bound,
    (bagof(M/C,(succ(N,M,C), \+member(M,P)),Succ),
    !,
    succlist(G,Succ,Ts),
    bestf(Ts,F1),
    expand(P,t(N,F1/G,Ts),Bound,Tree1,Solved,Sol);
    Solved = never    
    ).
expand(P,t(N,F/G,[T|Ts]),Bound,Tree1,Solved,Sol) :-
    F =< Bound,
    bestf(Ts,BF),min(Bound,BF,Bound1),
    expand([N|P],T,Bound1,T1,Solved1,Sol),
    continues(P,t(N,F/G,[T1|Ts]),Bound,Tree1,Solved1,Solved,Sol).
expand(_,t(_,_,[]),_,_,never,_):-!.
expand(_,Tree,Bound,Tree,no,_):-
    f(Tree,F),F>Bound.
continues(_,_,_,_,yes,yes,Sol).
continues(P,t(N,F/G,[T1|Ts]),Bound,Tree1,no,Solved,Sol):-
    insert(T1,Ts,NTs),
    bestf(NTs,F1),
    expand(P,t(N,F1/G,NTs),Bound,Tree1,Solved,Sol).
continues(P,t(N,F/G,[_|Ts]),Bound,Tree1,never,Solved,Sol):-
    bestf(Ts,F1),
    expand(P,t(N,F1/G,Ts),Bound,Tree1,Solved,Sol).
succlist(_,[],[]).
succlist(G0,[N/C|NCs],Ts):-
    G is G0 + C,
    h(N,H),
    F is G+H,
    succlist(G0,NCs,Ts1),
    insert(l(N,F/G),Ts1,Ts).

insert(T,Ts,[T|Ts]):-
    f(T,F),bestf(Ts,F1),
    F=<F1,!.
insert(T,[T1|Ts],[T1|Ts1]) :-
    insert(T,Ts,Ts1).
f(l(_,F/_),F).
f(t(_,F/_,_),F).
bestf([T|_],F) :-
    f(T,F).
bestf([],9999).


min(X,Y,M):-
    X < Y ->
    M is X;
    M is Y.
%% show_move(Plan):-write(Pa)
show_move([]).
show_move(Plan):- [A|B] = Plan,X->>Y = A,write(Y),nl,show_move(B).

begin():-
write('please enter the start state as a list like "[clear(b2),clear(2),clear(3),on(b2,b1),on(b1,b3),on(b3,1)]"'),nl,
read(N),
test(N).


test(N):- N == 1,
    consult('./Data1.pl'),
    bestfirst([on(b3,b1),on(b1,1),on(b2,2),clear(b3),clear(b2)]->>stop,Plan),
    !,show_move(Plan).
test(N):- N == 2,
    consult('./Data2.pl'),
    bestfirst([on(b2,b1),on(b1,b3),on(b3,2),on(b4,b5),on(b5,4)]->>stop,Plan),
    !,show_move(Plan).
test(N):- N == 3,
    consult('./Data3.pl'),
    bestfirst([on(b4,b3),on(b3,b5),on(b5,b1),on(b1,b2),on(b2,2)]->>stop,Plan),
    !,show_move(Plan).
test(N):- N == 4,
    consult('./Data4.pl'),
    bestfirst([on(b6,b2),on(b2,b4),on(b4,b1),on(b1,b3),on(b3,b5),on(b5,1)]->>stop,Plan),
    !,show_move(Plan).
test(N):- N == 5,
    consult('./Data5.pl'),
    bestfirst([on(b7,b2),on(b2,b4),on(b4,b1),on(b1,b3),on(b3,b6),on(b6,b8),on(b8,b5),on(b5,1)]->>stop,Plan),
    length(Plan,L),write(L),nl,
    !,show_move(Plan).
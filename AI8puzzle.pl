goal_state([1,2,3,4,5,6,7,8,_)).

move ([Blank,X,Y,Z,W,V,U,T,S], [X,Blank,Y,Z,W,V,U,T,S]).
move ([X,Blank,Y,Z,W,V,U,T,S], [X,Y,Blank,Z,W,V,U,T,S]).
move ([X,Y,Blank,Z,W,V,U,T,S], [X,Y,Z,Blank,W,V,U,T,S]).
move ([X,Y,Z,Blank,W,V,U,T,S], [X,Y,Z,W,Blank,V,U,T,S]).
move ([X,Y,Z,W,Blank,V,U,T,S], [X,Y,Z,W,V,Blank,U,T,S]).
move ([X,Y,Z,W,V,Blank,U,T,S], [X,Y,Z,W,V,U,Blank,T,S]).
move ([X,Y,Z,W,V,U,Blank,T,S], [X,Y,Z,W,V,U,T,Blank,S]).
move ([X,Y,Z,W,V,U,T,Blank,S]. [X,Y,Z,W,V,U,T,S,Blank]).

solve(State, Solution) :- 
      bfs([[state]|[]], Solution).

bfs ([[Node|Path]_], [Node|Path]) :
      goal state (Node),
      write('Goal state is found!'), nl.

bfs([Path|Paths], Solution) :-
      extend(Path, NewPaths),
      append (Paths, NewPaths, Pathsl),
      bfs(Pathsl, Solution).

extend([Node Path], NewPaths) :- 
findall (( NewNode, Node | Path),
         (move (Node, NewNode), \+member (NewNode, [Node|Path])),
          NewPaths).
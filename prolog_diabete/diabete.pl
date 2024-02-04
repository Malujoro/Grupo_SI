%   Paciente Sexo Idade Hipertensão *Cardíaco Fumante *IMC *Hemoglobina *Glicose Diabetes
paciente(['Maria',       feminino,  37.0, nao, nao, passado,    30.5,   5.7, 100], nao).
paciente(['Gorete',      feminino,  53.0, nao, nao, passado,    26.37,  4.5, 112], nao).
paciente(['Pedro',       masculino, 54.0, nao, nao, nunca,      31.86,  6.6, 145], nao).
paciente(['Juliana',     feminino,  37.0, nao, nao, passado,    21.25,  6.1, 200], nao).
paciente(['Catisvalda',  feminino,  56.0, nao, nao, nunca,      30.95,  6.5, 145], nao).
paciente(['Ursulino',    masculino, 79.0, nao, nao, passado,    31.19,  5.8, 155], nao).
paciente(['José',        masculino, 67.0, nao, nao, sim,        27.32,  6.8, 220], sim).
paciente(['Marta',       feminino,  74.0, nao, nao, sim,        29.51,  8.2, 240], sim).
paciente(['Jussara',     feminino,  37.0, nao, nao, passado,    36.87,  8.8, 160], sim).
paciente(['Katia',       feminino,  65.0, sim, sim, nunca,      33.55,  8.9, 202], sim).
paciente(['Marcelo',     masculino, 55.0, nao, sim, passado,    30.42,  6.2, 300], sim).
paciente(['Ayla',        feminino,  61.0, nao, nao, passado,    34.45,  6.5, 280], sim).
paciente(['Rafaela',     feminino,  58.0, sim, nao, nunca,      38.31,  7.0, 200], sim).
paciente(['Cássia',      feminino,  61.0, nao, nao, sim,        30.11,  6.2, 240], sim).
paciente(['Elisa',       feminino,  26.0, nao, nao, passado,    27.32,  4.0, 200], nao).
paciente(['Alice',       feminino,  74.0, nao, nao, passado,    28.12,  5.0, 100], nao).
paciente(['Jade',        feminino,  45.0, sim, nao, nunca,      23.05,  4.8, 130], nao).
paciente(['Heloise',     masculino, 56.0, nao, nao, nunca,      26.78,  4.8, 200], nao).
paciente(['Benício',     masculino, 20.0, nao, nao, passado,    23.04,  5.7, 160], nao).
paciente(['Paulo',       masculino, 70.0, nao, nao, passado,    15.94,  5.8, 158], nao).
paciente(['Otávio',      masculino, 30.0, nao, nao, passado,    15.8,   6.2, 90],  nao).
paciente(['Isis',        feminino,  80.0, nao, nao, nunca,      22.04,  9.0, 209], sim).
paciente(['Francivaldo', masculino, 63.0, nao, sim, passado,    27.32,  6.6, 300], sim).
paciente(['Juvelino',    masculino, 58.0, nao, nao, passado,    32.38,  6.6, 159], sim).
paciente(['Maya',        feminino,  43.0, sim, nao, nunca,      34.21,  9.0, 160], sim).
paciente(['Genetíldes',  masculino, 56.0, nao, nao, nunca,      37.59,  4.0, 159], nao).
paciente(['Waldisney',   masculino, 60.0, nao, nao, passado,    25.4,   4.0, 200], nao).
paciente(['Reinaldo',    masculino, 27.0, nao, nao, passado,    27.32,  3.5, 100], nao).
paciente(['Frederico',   masculino, 54.0, nao, nao, passado,    30.41,  5.0, 158], nao).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Regra para montar uma lista com os dados de todos os pacientes
listaPacientes(Lista) :-
    findall([[Nome, Sexo, Idade, Hipertensao, Cardiaco, Fumante, IMC, Hemoglobina, Glicose], Diabete],
        paciente([Nome, Sexo, Idade, Hipertensao, Cardiaco, Fumante, IMC, Hemoglobina, Glicose], Diabete),
        Lista
        ).

% Regra para calcular a média de uma lista
calculaMedia(Lista, Media) :-
    sumlist(Lista, Soma),
    length(Lista, Tam),
    Tam > 0,
    Media is Soma / Tam.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Regra para contar quantas vezes um "Elemento" aparece na lista
contaIgual([], _, 0) :- !.
contaIgual([X | T], Elemento, Resultado) :-
    contaIgual(T, Elemento, Resultado2),
    ((X = Elemento, !, Resultado is Resultado2 + 1)
    ;
    (!, Resultado is Resultado2)).

% Regra para contar quantos números maiores que o "Elemento" aparecem na lista
contaMaior([], _, 0) :- !.
contaMaior([X | T], Elemento, Resultado) :-
    contaMaior(T, Elemento, Resultado2),
    ((X > Elemento, !, Resultado is Resultado2 + 1)
    ;
    (!, Resultado is Resultado2)).

% Regra para contar quantos números menores que o "Elemento" aparecem na lista
contaMenor([], _, 0) :- !.
contaMenor([X | T], Elemento, Resultado) :-
    contaMenor(T, Elemento, Resultado2),
    ((X < Elemento, !, Resultado is Resultado2 + 1)
    ;
    (!, Resultado is Resultado2)).

% Regra para contar quantas vezes um "Elemento" aparece nos pacientes diagnosticados
contaIgualDiabetico([], [], _, 0) :- !.
contaIgualDiabetico([X | T], [X2 | T2], Elemento, Resultado) :-
    contaIgualDiabetico(T, T2, Elemento, Resultado2),
    ((X2 = sim, X = Elemento, !, Resultado is Resultado2 + 1)
    ;
    (!, Resultado is Resultado2)).

% Regra para contar quantos números maiores que o "Elemento" aparecem nos pacientes diagnosticados
contaMaiorDiabetico([], [], _, 0) :- !.
contaMaiorDiabetico([X | T], [X2 | T2], Elemento, Resultado) :-
    contaMaiorDiabetico(T, T2, Elemento, Resultado2),
    ((X2 = sim, X > Elemento, !, Resultado is Resultado2 + 1)
    ;
    (!, Resultado is Resultado2)).

% Regra para contar quantos números menores que o "Elemento" aparecem nos pacientes diagnosticados
contaMenorDiabetico([], [], _, 0) :- !.
contaMenorDiabetico([X | T], [X2 | T2], Elemento, Resultado) :-
    contaMenorDiabetico(T, T2, Elemento, Resultado2),
    ((X2 = sim, X < Elemento, !, Resultado is Resultado2 + 1)
    ;
    (!, Resultado is Resultado2)).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 

% Regra para "Montar" uma lista com o sexo de todos os pacientes
listaSexo([], []) :- !.
listaSexo([[[_, Sexo, _, _, _, _, _, _, _], _] | T], [Sexo | T2]) :- 
    listaSexo(T, T2).

% Regra para "Montar" uma lista com as Idades de todos os pacientes
listaIdade([], []) :- !.
listaIdade([[[_, _, Idade, _, _, _, _, _, _], _] | T], [Idade | T2]) :- 
    listaIdade(T, T2).

% Regra para "Montar" uma lista com a existência de Hipertensão de todos os pacientes
listaHipertensao([], []) :- !.
listaHipertensao([[[_, _, _, Hipertensao, _, _, _, _, _], _] | T], [Hipertensao | T2]) :- 
    listaHipertensao(T, T2).

% Regra para "Montar" uma lista com os IMCs de todos os pacientes
listaIMC([], []) :- !.
listaIMC([[[_, _, _, _, _, _, IMC, _, _], _] | T], [IMC | T2]) :- 
    listaIMC(T, T2).

% Regra para "Montar" uma lista com o índice de Hemoglobina de todos os pacientes
listaHemoglobina([], []) :- !.
listaHemoglobina([[[_, _, _, _, _, _, _, Hemoglobina, _], _] | T], [Hemoglobina | T2]) :- 
    listaHemoglobina(T, T2).

% Regra para "Montar" uma lista com o índice de Glicose de todos os pacientes
listaGlicose([], []) :- !.
listaGlicose([[[_, _, _, _, _, _, _, _, Glicose], _] | T], [Glicose | T2]) :- 
    listaGlicose(T, T2).

% Regra para "Montar" uma lista com a "posição" dos diabéticos
listaDiabete([], []) :- !.
listaDiabete([[_, Diabete] | T], [Diabete | T2]) :- 
    listaDiabete(T, T2).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% sintomas([Sexo, Idade, Hipertensao, Cardiaco, Fumante, IMC, Hemoglobina, Glicose])
% Serão calculados os "sintomas" ou fatores com mais chances de indicar diabetes.
% Será levado em consideração o fator com maior porcentagem de ocorrência.
% Por exemplo: [Feminino - 8/15 - 53,3% | Masculino - 4/14 - 28,5%], então aparentemente pessoas do gênero feminino têm mais chances de diabetes

    sintomas([Sexo, Hipertensao]) :-
    listaPacientes(Lista),
    listaDiabete(Lista, ListaDiabete),

    % Será calculado a quantidade de gente do sexo feminino e masculino cadastrado no sistema, bem como a porcentagem de diabéticos por gênero
    listaSexo(Lista, ListaSexo),
    contaIgual(ListaSexo, feminino, QuantFeminino),
    contaIgual(ListaSexo, masculino, QuantMasculino),
    contaIgualDiabetico(ListaSexo, ListaDiabete, feminino, QuantFeminino2),
    contaIgualDiabetico(ListaSexo, ListaDiabete, masculino, QuantMasculino2),
    write('Mulheres: '), write(QuantFeminino2), write('/'), write(QuantFeminino), nl,
    write('Homens: '), write(QuantMasculino2), write('/'), write(QuantMasculino), nl,
    ((QuantFeminino2/QuantFeminino >= QuantMasculino2/QuantMasculino, !, Sexo = feminino);(Sexo = masculino)),

    % Será calculado a quantidade de gente com hipertensão (ou não) cadastrado no sistema, bem como a porcentagem de diabéticos
    listaHipertensao(Lista, ListaHipertensao),
    contaIgual(ListaHipertensao, sim, SimHipertensao),
    contaIgual(ListaHipertensao, nao, NaoHipertensao),
    contaIgualDiabetico(ListaHipertensao, ListaDiabete, sim, SimHipertensao2),
    contaIgualDiabetico(ListaHipertensao, ListaDiabete, nao, NaoHipertensao2),
    write('Sim: '), write(SimHipertensao2), write('/'), write(SimHipertensao), nl,
    write('Não: '), write(NaoHipertensao2), write('/'), write(NaoHipertensao), nl,
    ((SimHipertensao2/SimHipertensao >= NaoHipertensao2/NaoHipertensao, !, Hipertensao = sim);(Hipertensao = nao)).


main() :-
    listaPacientes(Lista),
    
    listaDiabete(Lista, Diabete),
    write(Diabete), nl.
    % listaIdade(Lista, Idade),
    % calculaMedia(Idade, MediaIdade),
    
    % listaIMC(Lista, IMC),
    % calculaMedia(IMC, MediaIMC),

    % listaHemoglobina(Lista, Hemoglobina),
    % calculaMedia(Hemoglobina, MediaHemo),

    % listaGlicose(Lista, Glicose),
    % calculaMedia(Glicose, MediaGlico).

% adicionar_paciente([ConjuntoCaracteristicas], Diabetes).

% editar_paciente([ConjuntoCaracteristicas], Diabetes).

% remover_paciente([ConjuntoCaracteristicas], Diabetes).

% Regra para calcular o IMC de um paciente
calcular_IMC(Peso, Altura, IMC) :- 
    Peso > 0, Altura > 0, IMC is Peso / (Altura * Altura).

% diagnosticar_diabetes([ConjuntoCaracteristicas], Diabetes).

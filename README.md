BitParser
=========

This program was originally a homework for a job interview taken in 2004. I
didn't know C++ back then (and I still don't know), so the code isn't too
great, but it did its job nevertheless.

The purpose of the BitParser library is to iterate (parse) over a long stream
of bits and detect the *changes* from 0 to 1 and vice versa. For example the
stream `000011111100` has 2 changes. The stream `010101` has 5 changes. A
contiguous sequence of the same bit is called a *run*.

One of the requirements was to make the program as fast as possible, so instead
of testing an unsigned char (byte) at a time, I decided to test a whole word
since a computer works natively with words, not bytes. Unfortunately this
raises the problem of alignment. While for example on Linux/x86 you can read a
word from any address, the program crashed on Solaris/SPARC (with SIGBUS if I
remember correctly). So I had to modify the program accordingly.

The current version isn't very far from the original. I cleaned some of the
code and updated it to more modern standards i.e. 2011 ISO C++.

Building
--------

	make


Running the tests
-----------------

	./BitParserTest


Original Romanian text
======================

Programul face o cautare „obișnuită”, ținându-se cont și de cazurile în care un
byte nu are 8 biti.

Căutarea este împărțita în 3 etape:

 - ultima bucata din primul byte al locației în care se cauta
 - mijlocul liniei
 - prima bucata a ultimului byte al locației în care se cauta

Se utilizează o funcție auxiliară care caută un bit într-un (într-o bucată de)
byte.  Ea este utilizata pentru localizarea unui changing element.

Clasa stochează:

 - o referință către "linia" în care se cauta,
 - nr de biți
 - poziția ultimului changing element pentru a ști de unde să începem căutarea
 - valoarea ultimului bit cautat, pentru a ști cesăcautam

Pentru a ști când să ne oprim utilizăm o santinelă.

Pentru a optimiza cat mai mult, santinela este calculata la inițializare
(apelul constructorului).

O posibilă optimizare (puțin probabil să aibă efect asa de mare), este să
parcurgem run-urile la nivel de întreg (care are in mod tipic 32 de biți).
Avantajul unei astfel de parcurgeri consta în faptul ca hardware-ul oricum
lucrează la nivel de cuvânt și în loc să lucrăm cu bucăți de cuvânt, lucrăm cu
un cuvânt întreg. Dezavantajul constă în faptul că pointer-ul către întreg
trebuie să fie la o adresa multiplu de cuvânt ("aligned"), altfel putem avea
surpriza că programul să crape pe unele platforme (de ex: Sparc/Solaris).

În cazul meu, am lucrat cu o aliniere la nivel de long int, pentru a fi cat mai
sigur. Programul a fost testat atât pe x86/Linux, cat și pe Sparc/Solaris și a
mers fără probleme.

Lucrul la nivel de cuvânt a necesitat unele modificări, fiind necesare noi
variabile: santinela la nivel de cuvânt, cuvinte cu run-uri de 0 sau 1.

O descriere mai clara a algoritmului se găsește în comentariile din surse.

Ar mai fi de menționat și că se fac unele verificări ale parametrilor cu
assert, astfel încât varianta finală să nu aibă parte de vreun overhead.


Funcțiile de testare sunt parametrizate, astfel încât se pot face și niște mici
teste de performanta.

ATENȚIE! S-ar putea ca testele de performanță să dea greș datorită șirurilor
mari cu care se lucrează. Se rezolvă prin chemarea loc cu alți parametri (mai
mici).

Funcțiile de testare verifică doar pozițiile "changing" elementelor, ci și
clipa în care nu se mai găsesc asemenea elemente.


Pentru compilare se poate utiliza fie Makefile (Unix), fie Microsoft.makefile
(Visual C++).

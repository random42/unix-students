## Scelte progettuali

### Studenti

La matricola dello studente e' identificata dal suo pid.

### Random

La libreria di generazione di numeri casuale e' basata sul file "*/dev/urandom*" presente nei sistemi Unix.

### Memoria condivisa

Ogni studente e' pubblicato in una memoria condivisa che a cui ogni studente puo' accedere. Il campo *invite* e' TRUE se lo studente non fa parte di un gruppo chiuso. La lettura e scrittura della memoria e' regolata da 3 semafori.

### Flow del manager

La partenza viene stabilita dal semaforo *SEM_START*, inizializzato a 0. Ogni studente quando e' pronto lo incrementa di 1 e aspetta che divenga 0. Il manager decrementa di *POP_SIZE* il semaforo per dare il via, per cui deve aspettare che ogni studente sia pronto. A questo punto parte il timer di *SIM_TIME*.

Il manager non fa altro che ricevere messaggi di chiusura dei gruppi, salvando le informazioni sui vari gruppi. Allo scadere del timer stampa le tabelle dei voti e informazioni aggiuntive.

### Messaggi

Viene utilizzata un unica coda di messaggi per gli inviti, le risposte, la chiusura del gruppo e il conoscimento del voto finale.

### Sorting

L'algoritmo di sorting e' stata preso dal web, piu' specificamente e' il merge-sort usato nel kernel Linux.

## Strategia dello studente

Inizialmente ogni studente ordina gli studenti con stessa parita' di matricola per voto e quelli con voto piu' alto diventano i leader. Per stabilire con certezza quali sono i leader si sommano i *nof_elems* fino al raggiungimento del numero di studenti con stessa parita'. L'ultimo leader diminuira' il numero dei suoi elementi se necessario. Si tiene conto del numero di *nof_invites* nel caso non bastino per formare il gruppo desiderato. In quel caso il gruppo avra' meno elementi.

### Leader

I leader formano i loro gruppi a turno, tramite l'uso di un semaforo. Ogni leader ordina i non-leader per il miglioramento di voto che otterrebbero e invita i primi che non fanno parte di un gruppo chiuso. Attende le risposte (sempre positive) e procede a inviare il messaggio di chiusura del gruppo al gestore.

### Non-leader

Attendono un messaggio di invito e rispondono positivamente.

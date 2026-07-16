(function () {
    const RANKS   = ['Nine','Ten','Jack','Queen','King','Ace'];
    const SUITS   = ['Spades','Hearts','Clubs','Diamonds'];
    const SYM     = { Spades:'♠', Hearts:'♥', Clubs:'♣', Diamonds:'♦' };
    const SHORT   = { Nine:'9', Ten:'10', Jack:'J', Queen:'Q', King:'K', Ace:'A' };
    const RED     = new Set(['Hearts','Diamonds']);
    const NAMES   = ['You (South)','West','Partner (North)','East'];

    function sameColor(suit) {
        if (suit === 'Spades')   return 'Clubs';
        if (suit === 'Clubs')    return 'Spades';
        if (suit === 'Hearts')   return 'Diamonds';
        if (suit === 'Diamonds') return 'Hearts';
    }

    function isRightBower(card, trump) {
        return card.rank === 'Jack' && card.suit === trump;
    }

    function isLeftBower(card, trump) {
        return card.rank === 'Jack' && card.suit === sameColor(trump);
    }

    function isTrump(card, trump) {
        return card.suit === trump || isLeftBower(card, trump);
    }

    function effSuit(card, trump) {
        return isLeftBower(card, trump) ? trump : card.suit;
    }

    function rankW(rank) { return RANKS.indexOf(rank); }

    function cardLess(a, b, trump) {
        const at = isTrump(a, trump), bt = isTrump(b, trump);
        if (at !== bt) return !at;
        if (at && bt) {
            if (isRightBower(a, trump) !== isRightBower(b, trump))
                return !isRightBower(a, trump);
            if (isLeftBower(a, trump) !== isLeftBower(b, trump))
                return !isLeftBower(a, trump);
            return rankW(a.rank) < rankW(b.rank);
        }
        if (a.suit !== b.suit) return SUITS.indexOf(a.suit) < SUITS.indexOf(b.suit);
        return rankW(a.rank) < rankW(b.rank);
    }

    function cardLessTrick(a, b, led, trump) {
        const ls  = effSuit(led, trump);
        const af  = effSuit(a, trump) === ls;
        const bf  = effSuit(b, trump) === ls;
        if (af !== bf) {
            const at = isTrump(a, trump), bt = isTrump(b, trump);
            if (at !== bt) return !at;
            return !af;
        }
        return cardLess(a, b, trump);
    }

    function makeDeck() {
        const d = [];
        for (const s of SUITS)
            for (const r of RANKS)
                d.push({ rank: r, suit: s });
        return d;
    }

    function shuffle(deck) {
        for (let i = deck.length - 1; i > 0; i--) {
            const j = Math.floor(Math.random() * (i + 1));
            [deck[i], deck[j]] = [deck[j], deck[i]];
        }
    }

    function isFace(card) {
        return card.rank === 'Jack' || card.rank === 'Queen' ||
               card.rank === 'King' || card.rank === 'Ace';
    }

    function aiFaceCount(hand, trump) {
        return hand.filter(c => isTrump(c, trump) && isFace(c)).length;
    }

    function aiMakeTrump(hand, upcard, isDealer, round, original) {
        if (round === 1) {
            return aiFaceCount(hand, upcard.suit) >= 2 ? upcard.suit : null;
        }
        if (isDealer) {
            for (const s of SUITS) if (s !== original) return s;
        }
        for (const s of SUITS) {
            if (s === original) continue;
            if (aiFaceCount(hand, s) >= 1) return s;
        }
        return null;
    }

    function aiAddAndDiscard(hand, upcard, trump) {
        hand.push({ ...upcard });
        let worstIdx = -1, worst = null;
        for (let i = 0; i < hand.length; i++) {
            if (!isTrump(hand[i], trump)) {
                if (worstIdx === -1 || cardLess(hand[i], worst, trump)) {
                    worst = hand[i]; worstIdx = i;
                }
            }
        }
        if (worstIdx === -1) {
            for (let i = 0; i < hand.length; i++) {
                if (worstIdx === -1 || cardLess(hand[i], worst, trump)) {
                    worst = hand[i]; worstIdx = i;
                }
            }
        }
        hand.splice(worstIdx, 1);
    }

    function aiLead(hand, trump) {
        const nonT = hand.filter(c => !isTrump(c, trump));
        const pool = nonT.length > 0 ? nonT : hand;
        return pool.reduce((b, c) => cardLess(b, c, trump) ? c : b);
    }

    function aiPlay(hand, led, trump) {
        const ls       = effSuit(led, trump);
        const follows  = hand.filter(c => effSuit(c, trump) === ls);
        const pool     = follows.length > 0 ? follows : hand;
        return pool.reduce((b, c) => cardLess(b, c, trump) ? c : b);
    }

    let G = null;

    function newGame() {
        G = {
            hands:          [[], [], [], []],
            scores:         [0, 0],
            tricks:         [0, 0],
            trump:          null,
            upcard:         null,
            dealer:         1,
            ordered:        -1,
            upSuit:         null,
            pendingResolve: null,
            pendingType:    null,
        };
        log('Game started. Dealing…');
        renderScores();
        return G;
    }

    function waitFor(type) {
        return new Promise(resolve => {
            G.pendingType    = type;
            G.pendingResolve = resolve;
        });
    }

    function resolve(value) {
        if (!G || !G.pendingResolve) return;
        const fn = G.pendingResolve;
        G.pendingResolve = null;
        G.pendingType    = null;
        fn(value);
    }

    const pause = ms => new Promise(r => setTimeout(r, ms));

    const $ = id => document.getElementById(id);

    function log(msg) {
        const el = $('eu-log');
        el.insertAdjacentHTML('beforeend', msg + '<br>');
        el.scrollTop  = el.scrollHeight;
    }

    function cardHTML(card, playable = false, idx = -1) {
        const color = RED.has(card.suit) ? 'red' : 'black';
        const cls   = `eu-card ${color}${playable ? ' playable' : ''}`;
        const onclick = playable
            ? `onclick="euPlayCard(${idx})" `
            : '';
        return `<div class="${cls}" ${onclick}>
            <span class="card-rank">${SHORT[card.rank]}</span>
            <span class="suit-big">${SYM[card.suit]}</span>
        </div>`;
    }

    function backHTML() {
        return `<div class="eu-back"></div>`;
    }

    function renderHands() {
        const southPlayable = G.pendingType === 'play_card' || G.pendingType === 'discard';
        $('eu-hand-south').innerHTML = G.hands[0]
            .map((c, i) => cardHTML(c, southPlayable, i)).join('');

        ['eu-hand-west','eu-hand-north','eu-hand-east'].forEach((id, k) => {
            $(id).innerHTML = G.hands[k + 1].map(() => backHTML()).join('');
        });
    }

    function renderTrick(trick) {
        const slots = ['eu-t-north','eu-t-west','eu-t-east','eu-t-south'];
        const order = [2, 1, 3, 0];
        slots.forEach((id, k) => {
            const p = order[k];
            const entry = trick.find(e => e.player === p);
            $(id).innerHTML = entry ? cardHTML(entry.card) : '';
        });
    }

    function clearTrick() {
        ['eu-t-north','eu-t-west','eu-t-east','eu-t-south'].forEach(id => {
            $(id).innerHTML = '';
        });
    }

    function renderScores() {
        $('eu-score-you').textContent = G ? G.scores[0] : 0;
        $('eu-score-opp').textContent = G ? G.scores[1] : 0;
        $('eu-trump').textContent     = G && G.trump ? `${G.trump} ${SYM[G.trump]}` : '—';
        $('eu-tricks').textContent    = G ? `${G.tricks[0]} / ${G.tricks[1]}` : '— / —';
        $('eu-dealer').textContent    = G ? NAMES[G.dealer] : '—';
    }

    function setAction(html) {
        $('eu-action').innerHTML = html;
    }

    function showOrderUp(upcard) {
        const upcardHTML = cardHTML(upcard);
        $('eu-upcard-area').style.display = 'flex';
        $('eu-upcard-area').innerHTML     =
            `<span style="font-size:var(--fs-label);text-transform:uppercase;letter-spacing:.04em">Upcard:</span> ${upcardHTML}`;

        setAction(`
            <button class="site-btn accent" onclick="euOrderUp()">Order Up (${upcard.suit} ${SYM[upcard.suit]})</button>
            <button class="site-btn" onclick="euPass()">Pass</button>
        `);
    }

    function showPickSuit(blocked) {
        $('eu-upcard-area').style.display = 'none';
        const btns = SUITS
            .filter(s => s !== blocked)
            .map(s => `<button class="site-btn accent" onclick="euPickSuit('${s}')">${s} ${SYM[s]}</button>`)
            .join('');
        setAction(btns + (G.dealer !== 0
            ? `<button class="site-btn" onclick="euPass()">Pass</button>`
            : ''));
    }

    function showDiscard() {
        setAction(`<span style="font-size:var(--fs-label);">Pick a card to discard.</span>`);
    }

    function showNewHand() {
        $('eu-upcard-area').style.display = 'none';
        setAction(`<button class="site-btn accent" onclick="euStartHand()">New Hand</button>`);
    }

    async function euStartHand() {
        if (!G) newGame();
        G.hands   = [[], [], [], []];
        G.tricks  = [0, 0];
        G.trump   = null;
        G.upcard  = null;
        G.ordered = -1;

        const deck = makeDeck();
        shuffle(deck);
        let di = 0;

        for (let i = 0; i < 5; i++)
            for (let p = 0; p < 4; p++)
                G.hands[p].push(deck[di++]);

        G.upcard = deck[di];
        G.upSuit = G.upcard.suit;
        renderScores();
        renderHands();
        clearTrick();
        log(`--- Hand dealt. Dealer: ${NAMES[G.dealer]}. Upcard: ${G.upcard.rank} of ${G.upcard.suit} ---`);

        let trumpFound = false;
        const dealerLeft = (G.dealer + 1) % 4;

        for (let i = 0; i < 4 && !trumpFound; i++) {
            const p        = (dealerLeft + i) % 4;
            const isDealer = p === G.dealer;

            if (p === 0) {
                G.pendingType = 'trump1';
                showOrderUp(G.upcard);
                const choice = await waitFor('trump1');
                if (choice === 'order') {
                    log(`You ordered up ${G.upcard.suit}.`);
                    G.trump   = G.upcard.suit;
                    G.ordered = 0;
                    trumpFound = true;
                } else {
                    log('You passed.');
                }
            } else {
                await pause(500);
                const choice = aiMakeTrump(G.hands[p], G.upcard, isDealer, 1, null);
                if (choice) {
                    log(`${NAMES[p]} orders up ${G.upcard.suit}.`);
                    G.trump   = G.upcard.suit;
                    G.ordered = p;
                    trumpFound = true;
                } else {
                    log(`${NAMES[p]} passes.`);
                }
            }
        }

        if (!trumpFound) {
            for (let i = 0; i < 4 && !trumpFound; i++) {
                const p        = (dealerLeft + i) % 4;
                const isDealer = p === G.dealer;

                if (p === 0) {
                    showPickSuit(G.upSuit);
                    const choice = await waitFor('trump2');
                    if (choice !== 'pass') {
                        log(`You name ${choice} as trump.`);
                        G.trump   = choice;
                        G.ordered = 0;
                        trumpFound = true;
                    } else {
                        log('You passed (round 2).');
                    }
                } else {
                    await pause(500);
                    const choice = aiMakeTrump(G.hands[p], G.upcard, isDealer, 2, G.upSuit);
                    if (choice) {
                        log(`${NAMES[p]} names ${choice} as trump.`);
                        G.trump   = choice;
                        G.ordered = p;
                        trumpFound = true;
                    } else {
                        log(`${NAMES[p]} passes.`);
                    }
                }
            }
        }

        if (!trumpFound) {
            log('Everyone passed — redealing.');
            G.dealer = (G.dealer + 1) % 4;
            euStartHand();
            return;
        }

        renderScores();

        if (G.upcard && G.trump === G.upSuit) {
            log(`${NAMES[G.dealer]} picks up the upcard.`);
            if (G.dealer === 0) {
                G.hands[0].push({ ...G.upcard });
                G.pendingType = 'discard';
                showDiscard();
                renderHands();
                const discardIdx = await waitFor('discard');
                G.hands[0].splice(discardIdx, 1);
                log('You discarded a card.');
            } else {
                aiAddAndDiscard(G.hands[G.dealer], G.upcard, G.trump);
            }
        }
        $('eu-upcard-area').style.display = 'none';
        setAction('');
        log(`Trump: ${G.trump} ${SYM[G.trump]}`);
        renderHands();

        let leader = dealerLeft;
        for (let trickNum = 0; trickNum < 5; trickNum++) {
            clearTrick();
            const trick = [];
            log(`Trick ${trickNum + 1}:`);

            for (let i = 0; i < 4; i++) {
                const p   = (leader + i) % 4;
                const led = trick[0] ? trick[0].card : null;

                if (p === 0) {
                    if (led) {
                        const ls      = effSuit(led, G.trump);
                        const follows = G.hands[0].filter(c => effSuit(c, G.trump) === ls);
                        if (follows.length > 0) {
                            G.hands[0] = G.hands[0].map(c => ({
                                ...c, _mustFollow: effSuit(c, G.trump) === ls,
                            }));
                        }
                    }
                    G.pendingType = 'play_card';
                    setAction('<span style="font-size:var(--fs-label);">Your turn — click a card to play.</span>');
                    renderHands();
                    const idx  = await waitFor('play_card');
                    const card = G.hands[0].splice(idx, 1)[0];
                    trick.push({ player: 0, card });
                    log(`  You play ${card.rank} of ${card.suit}.`);
                } else {
                    await pause(600);
                    let card;
                    if (!led) {
                        card = aiLead(G.hands[p], G.trump);
                    } else {
                        card = aiPlay(G.hands[p], led, G.trump);
                    }
                    const ci = G.hands[p].indexOf(card);
                    G.hands[p].splice(ci, 1);
                    trick.push({ player: p, card });
                    log(`  ${NAMES[p]} plays ${card.rank} of ${card.suit}.`);
                }

                renderTrick(trick);
                renderHands();
            }

            let winner = trick[0];
            for (const t of trick)
                if (cardLessTrick(winner.card, t.card, trick[0].card, G.trump))
                    winner = t;

            const wTeam = winner.player % 2;
            G.tricks[wTeam]++;
            leader = winner.player;
            log(`  → ${NAMES[winner.player]} wins the trick. (${G.tricks[0]}-${G.tricks[1]})`);
            renderScores();
            await pause(900);
            clearTrick();
        }

        const ordTeam = G.ordered % 2;
        let pts0 = 0, pts1 = 0;

        if (G.tricks[ordTeam] >= 3) {
            if (G.tricks[ordTeam] === 5) {
                if (ordTeam === 0) { pts0 = 2; log('You and Partner march! +2 points.'); }
                else               { pts1 = 2; log('Opponents march! +2 points.'); }
            } else {
                if (ordTeam === 0) { pts0 = 1; log('You and Partner win the hand. +1 point.'); }
                else               { pts1 = 1; log('Opponents win the hand. +1 point.'); }
            }
        } else {
            if (ordTeam === 0) { pts1 = 2; log('Euchred! Opponents get 2 points.'); }
            else               { pts0 = 2; log('You euchred the opponents! +2 points.'); }
        }

        G.scores[0] += pts0;
        G.scores[1] += pts1;
        renderScores();

        if (G.scores[0] >= 10 || G.scores[1] >= 10) {
            const winner = G.scores[0] >= 10 ? 'You and Partner' : 'Opponents';
            log(`🎉 ${winner} win the game!`);
            setAction(`<button class="site-btn accent" onclick="euNewGame()">New Game</button>`);
        } else {
            G.dealer = (G.dealer + 1) % 4;
            showNewHand();
        }
    }

    window.euStartHand = function () {
        if (!G) newGame();
        $('eu-log').innerHTML = '';
        euStartHand();
    };

    window.euNewGame = function () {
        G = null;
        newGame();
        $('eu-log').innerHTML = '';
        renderHands();
        clearTrick();
        showNewHand();
        log('New game. Click <em>New Hand</em> to deal.');
    };

    window.euOrderUp = function () {
        if (G.pendingType !== 'trump1') return;
        resolve('order');
    };

    window.euPass = function () {
        const t = G.pendingType;
        if (t !== 'trump1' && t !== 'trump2') return;
        resolve('pass');
    };

    window.euPickSuit = function (suit) {
        if (G.pendingType !== 'trump2') return;
        resolve(suit);
    };

    window.euPlayCard = function (idx) {
        const t = G.pendingType;
        if (t !== 'play_card' && t !== 'discard') return;

        if (t === 'play_card') {
            const hasMust = G.hands[0].some(c => c._mustFollow);
            if (hasMust && !G.hands[0][idx]._mustFollow) {
                log('You must follow the led suit!');
                return;
            }
        }

        G.hands[0] = G.hands[0].map(({ _mustFollow, ...c }) => c);
        resolve(idx);
    };

    newGame();
    renderHands();
    clearTrick();
    showNewHand();
})();

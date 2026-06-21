(function () {
    const canvas  = document.getElementById('pokemon-canvas');
    const ctx     = canvas.getContext('2d');
    const stats   = document.getElementById('pokemon-stats');
    const optBtn  = document.getElementById('opt-btn');

    const DPR = window.devicePixelRatio || 1;
    canvas.width  = 700 * DPR;
    canvas.height = 420 * DPR;
    ctx.scale(DPR, DPR);

    const W = 700, H = 420;
    const PAD = 30;

    let points     = [];
    let animCancel = false;

    const C = { mst: '#3b82f6', fast: '#f97316', opt: '#22c55e', pt: '#0e0e0e' };

    function drawGrid() {
        ctx.clearRect(0, 0, W, H);
        ctx.strokeStyle = 'rgba(14,14,14,0.05)';
        ctx.lineWidth = 1;
        for (let x = PAD; x < W - PAD; x += 50) { ctx.beginPath(); ctx.moveTo(x, PAD); ctx.lineTo(x, H - PAD); ctx.stroke(); }
        for (let y = PAD; y < H - PAD; y += 50) { ctx.beginPath(); ctx.moveTo(PAD, y); ctx.lineTo(W - PAD, y); ctx.stroke(); }
    }

    function drawDots() {
        points.forEach((p, i) => {
            ctx.beginPath();
            ctx.arc(p.x, p.y, 6, 0, Math.PI * 2);
            ctx.fillStyle = C.pt;
            ctx.fill();
            ctx.fillStyle = '#fff';
            ctx.font = 'bold 10px Roboto, sans-serif';
            ctx.textAlign = 'center';
            ctx.textBaseline = 'middle';
            ctx.fillText(i, p.x, p.y);
        });
    }

    function redrawPoints() { drawGrid(); drawDots(); }

    function drawArrowedLine(x1, y1, x2, y2, color) {
        const dx = x2 - x1, dy = y2 - y1;
        const len = Math.sqrt(dx * dx + dy * dy);
        if (len < 1) return;
        const angle   = Math.atan2(dy, dx);
        const aLen    = 9;
        const aHalf   = 4;
        const spacing = 15;

        ctx.strokeStyle = color;
        ctx.lineWidth   = 0.8;
        ctx.globalAlpha = 0.25;
        ctx.setLineDash([]);
        ctx.beginPath();
        ctx.moveTo(x1, y1);
        ctx.lineTo(x2, y2);
        ctx.stroke();
        ctx.globalAlpha = 1;

        const count = Math.max(1, Math.floor(len / spacing));
        ctx.fillStyle = color;
        for (let i = 0; i < count; i++) {
            const t  = (i + 0.5) / count;
            const cx = x1 + dx * t;
            const cy = y1 + dy * t;
            ctx.save();
            ctx.translate(cx, cy);
            ctx.rotate(angle);
            ctx.beginPath();
            ctx.moveTo( aLen / 2, 0);
            ctx.lineTo(-aLen / 2,  aHalf);
            ctx.lineTo(-aLen / 2, -aHalf);
            ctx.closePath();
            ctx.fill();
            ctx.restore();
        }
    }

    const delay = ms => new Promise(r => setTimeout(r, ms));

    async function animateEdges(edges, color) {
        for (let i = 0; i < edges.length; i++) {
            if (animCancel) return;
            drawGrid();
            for (let j = 0; j <= i; j++) {
                const e = edges[j];
                drawArrowedLine(points[e[0]].x, points[e[0]].y,
                                points[e[1]].x, points[e[1]].y, color);
            }
            drawDots();
            await delay(300);
        }
    }

    async function animateTour(tour, color) {
        const n = tour.length;
        for (let i = 0; i < n; i++) {
            if (animCancel) return;
            drawGrid();
            for (let j = 0; j <= i; j++) {
                const a = points[tour[j]], b = points[tour[(j + 1) % n]];
                drawArrowedLine(a.x, a.y, b.x, b.y, color);
            }
            drawDots();
            await delay(220);
        }
    }

    function dist(a, b) {
        const dx = a.x - b.x, dy = a.y - b.y;
        return Math.sqrt(dx * dx + dy * dy);
    }

    function distMat(pts) {
        const n = pts.length;
        return Array.from({length: n}, (_, i) =>
            Array.from({length: n}, (_, j) => dist(pts[i], pts[j]))
        );
    }

    function primMST(pts) {
        const n = pts.length;
        if (n < 2) return { cost: 0, edges: [] };
        const inMST  = new Array(n).fill(false);
        const minD   = new Array(n).fill(Infinity);
        const parent = new Array(n).fill(-1);
        minD[0] = 0;
        const edges = [];
        let cost = 0;
        for (let iter = 0; iter < n; iter++) {
            let u = -1;
            for (let v = 0; v < n; v++)
                if (!inMST[v] && (u === -1 || minD[v] < minD[u])) u = v;
            inMST[u] = true;
            cost += minD[u];
            if (parent[u] !== -1) edges.push([parent[u], u]);
            for (let v = 0; v < n; v++) {
                if (!inMST[v]) {
                    const d = dist(pts[u], pts[v]);
                    if (d < minD[v]) { minD[v] = d; parent[v] = u; }
                }
            }
        }
        return { cost, edges };
    }

    function fastTSP(pts) {
        const n = pts.length;
        if (n === 0) return { cost: 0, tour: [] };
        if (n === 1) return { cost: 0, tour: [0] };
        if (n === 2) return { cost: dist(pts[0], pts[1]) * 2, tour: [0, 1] };

        let far = 1, maxD = dist(pts[0], pts[1]);
        for (let i = 2; i < n; i++) {
            const d = dist(pts[0], pts[i]);
            if (d > maxD) { maxD = d; far = i; }
        }
        const tour   = [0, far];
        const inTour = new Set([0, far]);

        while (inTour.size < n) {
            let best = -1, bestD = -1;
            for (let v = 0; v < n; v++) {
                if (inTour.has(v)) continue;
                let minDToTour = Infinity;
                for (const t of tour) minDToTour = Math.min(minDToTour, dist(pts[v], pts[t]));
                if (minDToTour > bestD) { bestD = minDToTour; best = v; }
            }
            let bestPos = 0, bestInc = Infinity;
            for (let i = 0; i < tour.length; i++) {
                const a = tour[i], b = tour[(i + 1) % tour.length];
                const inc = dist(pts[a], pts[best]) + dist(pts[best], pts[b]) - dist(pts[a], pts[b]);
                if (inc < bestInc) { bestInc = inc; bestPos = i + 1; }
            }
            tour.splice(bestPos, 0, best);
            inTour.add(best);
        }

        let cost = 0;
        for (let i = 0; i < tour.length; i++)
            cost += dist(pts[tour[i]], pts[tour[(i + 1) % tour.length]]);
        return { cost, tour };
    }

    function optTSP(pts) {
        const n = pts.length;
        if (n <= 2) return fastTSP(pts);
        const d = distMat(pts);
        let best = fastTSP(pts);
        let bestCost = best.cost;
        let bestTour = [...best.tour];
        const visited = new Array(n).fill(false);
        visited[0] = true;

        function lowerBound(path, curCost) {
            if (path.length === n) return curCost + d[path[n - 1]][0];
            const last = path[path.length - 1];
            let extra = Infinity;
            for (let v = 0; v < n; v++)
                if (!visited[v]) extra = Math.min(extra, d[last][v]);
            if (extra === Infinity) extra = 0;
            let mstEst = extra;
            for (let v = 0; v < n; v++) {
                if (!visited[v]) {
                    let minEdge = d[v][0];
                    for (let u = 0; u < n; u++)
                        if (!visited[u] && u !== v) minEdge = Math.min(minEdge, d[v][u]);
                    mstEst += minEdge;
                }
            }
            return curCost + mstEst / 2;
        }

        function bb(path, curCost) {
            if (path.length === n) {
                const total = curCost + d[path[n - 1]][0];
                if (total < bestCost) { bestCost = total; bestTour = [...path]; }
                return;
            }
            for (let v = 1; v < n; v++) {
                if (!visited[v]) {
                    const newCost = curCost + d[path[path.length - 1]][v];
                    if (lowerBound([...path, v], newCost) < bestCost) {
                        visited[v] = true;
                        path.push(v);
                        bb(path, newCost);
                        path.pop();
                        visited[v] = false;
                    }
                }
            }
        }

        bb([0], 0);
        return { cost: bestCost, tour: bestTour };
    }

    window.pokemonClick = function (e) {
        animCancel = true;
        const rect = canvas.getBoundingClientRect();
        const scaleX = W / rect.width, scaleY = H / rect.height;
        points.push({
            x: (e.clientX - rect.left) * scaleX,
            y: (e.clientY - rect.top)  * scaleY,
        });
        redrawPoints();
        stats.textContent = `${points.length} point${points.length !== 1 ? 's' : ''} placed. Run an algorithm.`;
        optBtn.disabled = points.length > 12;
    };

    window.pokemonMST = async function () {
        animCancel = true;
        await delay(10);
        animCancel = false;
        if (points.length < 2) { stats.textContent = 'Place at least 2 points.'; return; }
        const { cost, edges } = primMST(points);
        stats.textContent = 'Building MST…';
        await animateEdges(edges, C.mst);
        if (!animCancel) stats.textContent = `MST cost: ${cost.toFixed(1)} px`;
    };

    window.pokemonFastTSP = async function () {
        animCancel = true;
        await delay(10);
        animCancel = false;
        if (points.length < 3) { stats.textContent = 'Place at least 3 points.'; return; }
        const { cost, tour } = fastTSP(points);
        stats.textContent = 'Drawing FastTSP tour…';
        await animateTour(tour, C.fast);
        if (!animCancel) stats.textContent = `FastTSP tour length: ${cost.toFixed(1)} px`;
    };

    window.pokemonOptTSP = async function () {
        animCancel = true;
        await delay(10);
        animCancel = false;
        if (points.length < 3) { stats.textContent = 'Place at least 3 points.'; return; }
        if (points.length > 12) { stats.textContent = 'OptTSP requires ≤ 12 points.'; return; }
        stats.textContent = 'Computing optimal tour…';
        const { cost, tour } = optTSP(points);
        await animateTour(tour, C.opt);
        if (!animCancel) stats.textContent = `OptTSP tour length: ${cost.toFixed(1)} px (exact)`;
    };

    window.pokemonClear = function () {
        animCancel = true;
        points = [];
        drawGrid();
        stats.textContent = 'Canvas cleared. Click to place points.';
        optBtn.disabled = false;
    };

    redrawPoints();
})();

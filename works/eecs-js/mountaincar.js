(function () {
    const canvas   = document.getElementById('mc-canvas');
    const ctx      = canvas.getContext('2d');
    const statsEl  = document.getElementById('mc-stats');
    const trainBtn = document.getElementById('mc-train-btn');
    const watchBtn = document.getElementById('mc-watch-btn');
    const resetBtn = document.getElementById('mc-reset-btn');

    const DPR = window.devicePixelRatio || 1;
    const W = 700, H = 280;
    canvas.width  = W * DPR;
    canvas.height = H * DPR;
    ctx.scale(DPR, DPR);

    const POS_MIN = -1.2, POS_MAX = 0.6;
    const VEL_MIN = -0.07, VEL_MAX = 0.07;
    const GOAL    = 0.5;
    const BINS    = 20;
    const ACTIONS = 3;
    const MAX_STEPS = 200;

    let Q = null;
    let trained = false;
    let watching = false;
    let watchFrame = null;

    function initQ() {
        Q = Array.from({length: BINS}, () =>
            Array.from({length: BINS}, () =>
                Array.from({length: ACTIONS}, () => (Math.random() - 0.5) * 0.01)
            )
        );
        trained  = false;
        watching = false;
        if (watchFrame) cancelAnimationFrame(watchFrame);
        watchBtn.disabled = true;
    }

    function discretize(pos, vel) {
        const pi = Math.min(BINS - 1, Math.max(0,
            Math.floor((pos - POS_MIN) / (POS_MAX - POS_MIN) * BINS)));
        const vi = Math.min(BINS - 1, Math.max(0,
            Math.floor((vel - VEL_MIN) / (VEL_MAX - VEL_MIN) * BINS)));
        return [pi, vi];
    }

    function step(pos, vel, action) {
        vel = vel + (action - 1) * 0.001 - 0.0025 * Math.cos(3 * pos);
        vel = Math.max(VEL_MIN, Math.min(VEL_MAX, vel));
        pos = Math.max(POS_MIN, Math.min(POS_MAX, pos + vel));
        if (pos <= POS_MIN && vel < 0) vel = 0;
        const done   = pos >= GOAL;
        const reward = done ? 100 : -1;
        return { pos, vel, reward, done };
    }

    function reset() {
        const pos = -0.5 + (Math.random() - 0.5) * 0.2;
        const vel = 0;
        return { pos, vel };
    }

    function mcTrain() {
        if (watchFrame) cancelAnimationFrame(watchFrame);
        watching = false;
        if (!Q) initQ();
        const EPISODES = 5000;
        const lr   = parseFloat(document.getElementById('mc-lr').value);
        const γ    = parseFloat(document.getElementById('mc-gamma').value);
        let epsilon = 0.5;
        const epsilonDecay = epsilon / EPISODES;
        let bestReward = -Infinity;

        trainBtn.disabled = true;
        watchBtn.disabled = true;
        resetBtn.disabled = true;
        drawIdle();

        function trainBatch(startEp) {
            const BATCH = 200;
            for (let ep = startEp; ep < Math.min(startEp + BATCH, EPISODES); ep++) {
                let { pos, vel } = reset();
                let totalReward = 0;

                for (let t = 0; t < MAX_STEPS; t++) {
                    const [pi, vi] = discretize(pos, vel);

                    let action;
                    if (Math.random() < epsilon) {
                        action = Math.floor(Math.random() * ACTIONS);
                    } else {
                        action = argmax(Q[pi][vi]);
                    }

                    const { pos: np, vel: nv, reward, done } = step(pos, vel, action);
                    const [npi, nvi] = discretize(np, nv);

                    const target = reward + (done ? 0 : γ * Math.max(...Q[npi][nvi]));
                    Q[pi][vi][action] += lr * (target - Q[pi][vi][action]);

                    totalReward += reward;
                    pos = np; vel = nv;
                    if (done) break;
                }

                if (totalReward > bestReward) bestReward = totalReward;
                epsilon = Math.max(0.01, epsilon - epsilonDecay);
            }

            const ep = Math.min(startEp + BATCH, EPISODES);
            statsEl.textContent = `Training… Episode ${ep} / ${EPISODES}  ·  Best reward: ${bestReward.toFixed(0)}`;

            if (ep < EPISODES) {
                setTimeout(() => trainBatch(ep), 0);
            } else {
                trained  = true;
                trainBtn.disabled = false;
                watchBtn.disabled = false;
                resetBtn.disabled = false;
                statsEl.textContent = `Training complete! Best reward: ${bestReward.toFixed(0)}. Click Watch Agent to see the policy.`;
            }
        }

        trainBatch(0);
    }

    function mcWatch() {
        if (!trained) return;
        watching = true;
        watchBtn.disabled = true;

        let { pos, vel } = reset();
        let step_n = 0;
        let totalR = 0;

        function frame() {
            const [pi, vi] = discretize(pos, vel);
            const action = argmax(Q[pi][vi]);
            const res = step(pos, vel, action);
            pos = res.pos; vel = res.vel; totalR += res.reward; step_n++;

            drawCar(pos, vel);

            if (res.done) {
                statsEl.textContent = `Reached the goal in ${step_n} steps! Reward: ${totalR.toFixed(0)}`;
                watchBtn.disabled = false;
                watching = false;
                return;
            }
            if (step_n >= MAX_STEPS) {
                statsEl.textContent = `Episode ended after ${MAX_STEPS} steps. Reward: ${totalR.toFixed(0)}. Try training again or re-watch.`;
                watchBtn.disabled = false;
                watching = false;
                return;
            }
            watchFrame = requestAnimationFrame(frame);
        }
        watchFrame = requestAnimationFrame(frame);
    }

    function mcReset() {
        if (watchFrame) cancelAnimationFrame(watchFrame);
        initQ();
        statsEl.textContent = 'Q-table reset. Click Train to start.';
        trainBtn.disabled = false;
        resetBtn.disabled = false;
        drawIdle();
    }

    const PAD = 40;

    function mountainY(pos) {
        const h = Math.sin(3 * pos);
        const bottom = H - 20;
        const top    = H / 3;
        return bottom - (h + 1) / 2 * (bottom - top);
    }

    function posToCanvasX(pos) {
        return PAD + ((pos - POS_MIN) / (POS_MAX - POS_MIN)) * (W - 2 * PAD);
    }

    function drawMountainProfile(alpha = 1) {
        ctx.save();
        ctx.globalAlpha = alpha;
        ctx.beginPath();
        ctx.moveTo(PAD, H);
        for (let px = 0; px <= W - 2 * PAD; px++) {
            const pos = POS_MIN + (px / (W - 2 * PAD)) * (POS_MAX - POS_MIN);
            ctx.lineTo(PAD + px, mountainY(pos));
        }
        ctx.lineTo(W - PAD, H);
        ctx.closePath();
        ctx.fillStyle = '#d1d5db';
        ctx.fill();
        ctx.strokeStyle = '#9ca3af';
        ctx.lineWidth = 1.5;
        ctx.stroke();

        const gx = posToCanvasX(GOAL);
        const gy = mountainY(GOAL);
        ctx.strokeStyle = '#22c55e';
        ctx.lineWidth = 1.5;
        ctx.beginPath(); ctx.moveTo(gx, gy); ctx.lineTo(gx, gy - 20); ctx.stroke();
        ctx.fillStyle = '#22c55e';
        ctx.beginPath();
        ctx.moveTo(gx, gy - 20); ctx.lineTo(gx + 10, gy - 16); ctx.lineTo(gx, gy - 12);
        ctx.fill();

        ctx.restore();
    }

    function drawIdle() {
        ctx.clearRect(0, 0, W, H);
        ctx.fillStyle = '#f9fafb';
        ctx.fillRect(0, 0, W, H);
        drawMountainProfile();
        ctx.fillStyle = 'rgba(14,14,14,0.4)';
        ctx.font = '13px Roboto, sans-serif';
        ctx.textAlign = 'center';
        ctx.fillText('Train an agent to drive the car up the hill →', W / 2, 24);
    }

    function drawCar(pos, vel) {
        ctx.clearRect(0, 0, W, H);
        ctx.fillStyle = '#f9fafb';
        ctx.fillRect(0, 0, W, H);
        drawMountainProfile();

        const cx = posToCanvasX(pos);
        const cy = mountainY(pos);

        const dpos = 0.02;
        const angle = Math.atan2(
            mountainY(pos + dpos) - mountainY(pos - dpos),
            posToCanvasX(pos + dpos) - posToCanvasX(pos - dpos)
        );

        ctx.save();
        ctx.translate(cx, cy);
        ctx.rotate(angle);
        ctx.fillStyle = '#bb2649';
        ctx.fillRect(-14, -8, 28, 8);
        ctx.fillStyle = '#0e0e0e';
        ctx.beginPath(); ctx.arc(-8, 1, 4, 0, Math.PI * 2); ctx.fill();
        ctx.beginPath(); ctx.arc(8,  1, 4, 0, Math.PI * 2); ctx.fill();
        ctx.restore();

        ctx.fillStyle = 'rgba(14,14,14,0.08)';
        ctx.fillRect(PAD, 8, W - 2*PAD, 10);
        const barW = ((vel - VEL_MIN) / (VEL_MAX - VEL_MIN)) * (W - 2*PAD);
        ctx.fillStyle = vel > 0 ? '#22c55e' : '#bb2649';
        ctx.fillRect(PAD, 8, barW, 10);
        ctx.fillStyle = 'rgba(14,14,14,0.5)';
        ctx.font = '9px Roboto, sans-serif';
        ctx.textAlign = 'left';
        ctx.fillText(`vel ${vel.toFixed(3)}  pos ${pos.toFixed(3)}`, PAD, 7);
    }

    function argmax(arr) {
        let best = 0;
        for (let i = 1; i < arr.length; i++) if (arr[i] > arr[best]) best = i;
        return best;
    }

    window.mcTrain = mcTrain;
    window.mcWatch = mcWatch;
    window.mcReset = mcReset;

    initQ();
    drawIdle();
})();

document.addEventListener('DOMContentLoaded', () => {
    document.querySelectorAll('a[href]').forEach(link => {
        const href = link.getAttribute('href');
        const target = link.getAttribute('target');

        // Skip: external (http/https), new-tab, anchors, mailto, tel
        if (!href ||
            href.startsWith('http') ||
            href.startsWith('mailto') ||
            href.startsWith('#') ||
            href.startsWith('tel') ||
            target === '_blank') return;

        link.addEventListener('click', e => {
            if (e.metaKey || e.ctrlKey || e.shiftKey || e.altKey) return;
            e.preventDefault();
            window.location.href = link.href;
        });
    });
});

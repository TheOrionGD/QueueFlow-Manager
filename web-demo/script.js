document.addEventListener('DOMContentLoaded', () => {
    // Terminal animation sequence
    const terminalLines = [
        { type: 'input', text: './waiting_list' },
        { type: 'output', text: 'Initializing QueueFlow Manager...' },
        { type: 'output', text: 'Loading previous data from bookings.dat...' },
        { type: 'output', text: 'Waitlist data restored successfully.' },
        { type: 'output', text: 'Seats Capacity: 15' },
        { type: 'input', text: '2' },
        { type: 'output', text: 'Enter seat number to book (1-15): 3' },
        { type: 'output', text: 'Enter passenger name: John Doe' },
        { type: 'output', text: 'Is this a VIP booking? (1 for Yes, 0 for No): 1' },
        { type: 'output', text: 'Added John Doe (VIP) to the waiting list for seat 3.' },
        { type: 'input', text: '11' },
        { type: 'output', text: 'Enter filename for export (e.g. data.csv): export.csv' },
        { type: 'output', text: 'Successfully exported bookings and waitlist to export.csv' }
    ];

    const terminalBody = document.getElementById('terminal-body');
    terminalBody.innerHTML = ''; // Clear initial static content

    let lineIndex = 0;

    function typeLine(lineData) {
        if (lineIndex >= terminalLines.length) {
            // Add final blinking prompt
            const finalPrompt = document.createElement('div');
            finalPrompt.className = 'line typing';
            finalPrompt.innerHTML = '<span class="prompt">QueueFlow></span>';
            terminalBody.appendChild(finalPrompt);
            return;
        }

        const div = document.createElement('div');
        div.className = lineData.type === 'input' ? 'line typing' : 'line output';

        if (lineData.type === 'input') {
            div.innerHTML = '<span class="prompt">QueueFlow></span>';
        }

        terminalBody.appendChild(div);

        let charIndex = 0;
        function typeChar() {
            if (charIndex < lineData.text.length) {
                div.innerHTML += lineData.text.charAt(charIndex);
                charIndex++;
                setTimeout(typeChar, 30); // Typing speed
            } else {
                div.classList.remove('typing');
                lineIndex++;
                setTimeout(() => typeLine(terminalLines[lineIndex]), 300); // Wait before next line
            }
        }

        typeChar();
    }

    // Start animation when scrolled into view
    const observer = new IntersectionObserver((entries) => {
        entries.forEach(entry => {
            if (entry.isIntersecting) {
                setTimeout(() => typeLine(terminalLines[lineIndex]), 500);
                observer.unobserve(entry.target);
            }
        });
    }, { threshold: 0.5 });

    observer.observe(document.querySelector('.terminal-section'));

    // Glitch effect on h1
    const glitchElement = document.querySelector('.glitch');
    setInterval(() => {
        glitchElement.style.transform = `translate(${Math.random() * 4 - 2}px, ${Math.random() * 4 - 2}px)`;
        setTimeout(() => {
            glitchElement.style.transform = 'translate(0, 0)';
        }, 50);
    }, 3000);
});

import { defineConfig } from "vite";
import react from "@vitejs/plugin-react";

// https://vite.dev/config/
export default defineConfig({
  plugins: [react()],
  build: {
    outDir: "../data", // Set the output directory to 'data'
    rollupOptions: {
      input: "index.html", // Ensure this points to your main HTML file
      output: {
        entryFileNames: "script.js", // Customize JavaScript output
        assetFileNames: () => {
          // Separate CSS files into the same 'assets' folder
          return "styles.css";
        },
      },
    },
    emptyOutDir: true, // Clears the output directory before building
  },
  preview: {
    port: 5173,
    host: true,
    strictPort: true,
  },
  server: {
    watch: {
      usePolling: true,
    },
    host: true,
    strictPort: true,
    port: 5173,
  },
});

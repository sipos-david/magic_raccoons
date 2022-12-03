import { createContext, useContext, useState, ReactNode, useEffect, Fragment } from "react";

export type SnackbarMessage = {
  severity?: "error" | "success" | "info" | "warning",
  text: string,
}

type SnackbarData = {
  message: SnackbarMessage,
  key: number,
}

interface UseSnackbarProps {
  showSnackbar: (data: SnackbarMessage) => void;
}

interface SnackbarProviderProps {
  children?: ReactNode
}

const SnackbarContext = createContext<UseSnackbarProps | undefined>(undefined);
const defaultContext: UseSnackbarProps = { showSnackbar: () => { /* Intentionally empty */ }, };

export const useSnackbar = () => useContext(SnackbarContext) ?? defaultContext;

const SimpleSnackbar = ({ children }: SnackbarProviderProps) => {
  const [snackQueue, setSnackQueue] = useState<readonly SnackbarData[]>([]);
  const [snackbar, setSnackbar] = useState<SnackbarData | undefined>(undefined);
  const [isOpen, setOpen] = useState(false);

  function onShowSnackbar(msg: SnackbarMessage) {
    setSnackQueue((prev) => [...prev, { message: msg, key: new Date().getTime() }]);
  }

  function handleClose() {
    setOpen(false);
  }

  useEffect(() => {
    if (snackQueue.length && (!snackbar || !isOpen)) {
      // Set a new snack when we don't have an active one
      setSnackbar({ ...snackQueue[0] });
      setSnackQueue((prev) => prev.slice(1));
      setOpen(true);
    } else if (snackQueue.length && snackbar && isOpen) {
      // Close an active snack when a new one is added
      setOpen(false);
    }
  }, [snackQueue, snackbar, isOpen]);

  const snackOpen = isOpen && !!snackbar?.message.text;
  const snackSeverity = snackbar?.message.severity ?? "info";

  return (
    <SnackbarContext.Provider value={{ showSnackbar: onShowSnackbar }} >
      {children}
      {snackOpen &&
        <div className="fixed bottom-4 left-4 max-w-lg mx-auto">
          <SnackBar severity={snackSeverity} text={snackbar?.message.text} onClose={handleClose} />
        </div>
      }
    </SnackbarContext.Provider>
  );
};

export const SnackbarProvider = (props: SnackbarProviderProps) => {
  const context = useContext(SnackbarContext);

  // Ignore nested context providers, just passthrough children
  if (context) return <Fragment>{props.children}</Fragment>;
  return <SimpleSnackbar {...props} />;
};

export type SnackbarProps = {
  severity: "error" | "success" | "info" | "warning",
  text: string,
  onClose: () => void,
}

function SnackBar({ severity, text, onClose }: SnackbarProps) {
  if (severity === "error") {
    return (
      <div className="flex bg-red-100 rounded-lg p-4 text-sm text-red-700" role="alert">
        <span className="font-medium mr-2">Hiba!</span> {text}
        <button className="font-large ml-2 font-bold" onClick={onClose}>X</button>
      </div>
    );
  }

  if (severity === "success") {
    return (
      <div className="flex bg-green-100 rounded-lg p-4 text-sm text-green-700" role="alert">
        <span className="font-medium mr-2">Siker!</span> {text}
        <button className="font-large ml-2 font-bold" onClick={onClose}>X</button>
      </div>
    );
  }

  if (severity === "warning") {
    return (
      <div className="flex bg-yellow-100 rounded-lg p-4 text-sm text-yellow-700" role="alert">
        <span className="font-medium mr-2">Figyelem!</span> {text}
        <button className="font-large ml-2 font-bold" onClick={onClose}>X</button>
      </div>
    );
  }

  return (
    <div className="flex bg-blue-100 rounded-lg p-4 text-sm text-blue-700" role="alert">
      <span className="font-medium mr-2">Info!</span> {text}
      <button className="font-large ml-2 font-bold" onClick={onClose}>X</button>
    </div>
  );
}
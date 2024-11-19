export interface CanFrame {
  id: number;
  length: number;
  data: number[];
  isRTR: number;
  isExtended: number;
  frequency: number;
}

export interface CanMessage extends CanFrame {
  name: string;
  signals: CanMessageSignal[];
}

export interface CanMessageSignal {
  name: string;
  value: number;
  unit: string;
}

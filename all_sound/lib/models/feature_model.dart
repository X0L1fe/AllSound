class AudioFeatures {
  final double bpm;
  final double energy;
  final String key;

  final List<double> energyCurve;
  final List<double> onsetStrength;
  final List<bool> silenceMap;

  final double duration;

  AudioFeatures({
    required this.bpm,
    required this.energy,
    required this.key,
    required this.energyCurve,
    required this.onsetStrength,
    required this.silenceMap,
    required this.duration,
  });

  Map<String, dynamic> toJson() => {
    "bpm": bpm,
    "energy": energy,
    "key": key,
    "energy_curve": energyCurve,
    "onset_strength": onsetStrength,
    "silence_map": silenceMap,
    "duration": duration,
  };
}
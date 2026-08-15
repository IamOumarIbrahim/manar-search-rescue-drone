# Quality benchmark: *Unsupervised Face Detection in the Dark* vs. MANAR

## Purpose and standard of comparison

This document uses Wenjing Wang et al., *Unsupervised Face Detection in the Dark* (IEEE TPAMI, DOI 10.1109/TPAMI.2022.3152562), as a benchmark for the MANAR paper in `docs/paper/main.tex` and `docs/paper/main.pdf`.

The goal is not to copy the face-detection paper's wording, method, figures, publisher marks, or claims. The goal is to copy its research discipline: a sharply framed problem, a causal method, fair comparisons, reproducible experiments, ablations, failure analysis, honest limits, and an economical IEEE presentation. MANAR is a systems/pre-flight prototype paper, whereas the reference is a mature computer-vision method paper. MANAR should therefore imitate the reference's reasoning and evidentiary standard, not pretend to be the same kind of contribution.

This audit distinguishes three things:

1. **Scholarly quality** - the quality of the question, evidence, argument, and limitations.
2. **Editorial quality** - title, abstract, narrative order, figures, tables, captions, typography, and page economy.
3. **Publisher production** - TPAMI running heads, page numbers 1250-1266, copyright lines, manuscript-history footnotes, and the journal's final trim size. These are not evidence of research quality and should not be imitated by an unpublished preprint.

## Baseline snapshot before revision

The following is a measured comparison of the supplied PDFs before applying the changes recommended below. Text-extraction counts are approximate because PDF extraction can split ligatures and mathematical symbols.

| Attribute | Reference paper | MANAR before revision | What the difference means |
|---|---:|---:|---|
| Publication state | Peer-reviewed TPAMI article | Project technical preprint | The reference has passed several rounds of expert review and publisher production; MANAR must label itself honestly as a preprint. |
| Pages | 17 | 11 | Length alone is not quality, but the reference devotes substantially more space to method validation and analysis. |
| Extracted words | approximately 12,983 | approximately 6,533 | MANAR is not merely shorter; a larger fraction of its space is architecture inventory rather than experimental reasoning. |
| Figures | 20 | 6 | The reference repeatedly shows the mechanism, comparisons, distributions, qualitative results, failures, and applications. MANAR has few direct visual comparisons. |
| Tables | 9 | 10 | MANAR has many tables, but several are requirements/status inventories. The reference uses most tables to answer experimental questions. |
| Numbered equations | 19 | 8 | The reference's equations define the proposed method and its training objectives. Several MANAR equations describe standard background models rather than a novel evaluated mechanism. |
| Distinct numbered references | 80 | 12 | The reference establishes a deep research neighborhood. MANAR's literature coverage is currently too thin for its broad claims. |
| Main experimental comparison | 24 methods in five categories, plus a supervised upper bound | Two detector models in a diagnostic clip benchmark | MANAR's comparison set is too small to support architecture-wide conclusions. |
| Ablation studies | Extensive component, curve, model-size, and objective ablations | None in the submitted PDF | MANAR cannot yet show which design decisions cause its results. |
| Failure analysis | Dedicated failure cases with qualitative examples | General limitations, but no detector failure panel | MANAR discusses possible risks without tying them to observed cases. |
| Generalization tests | Face detection, generic detection, classification, segmentation, real-world cases | One local clip corpus and software design discussion | The reference tests whether the principle transfers; MANAR does not yet establish external validity. |
| Page size | 564.831 x 771.08 pt (publisher trim) | 612 x 792 pt (US Letter) | This is a venue-production difference, not a reason to override IEEE margins manually. |
| Approximate rendered outer ink margins | publisher furniture reaches about 0.25 in from edges | about 0.67 in left/right, 0.75 in top, 1.0 in bottom | The reference includes running heads, footers, and copyright furniture. Compare body readability and column rhythm, not the outermost ink box. |
| Title length/focus | One clear technical problem | Three contribution types joined in one long title | MANAR's title signals a bundle of subsystems instead of a single thesis. |
| Abstract style | Problem -> gap -> method -> mechanism -> result -> scope | Inventory -> several numerical claims -> future-work disclaimer | MANAR needs a clearer argument and must remove metrics not supported by its artifacts. |

## Why the reference paper is a great paper, and how MANAR should use it

1. **It begins with one precise, consequential research problem.** The paper asks how a face detector can be adapted from normal light to darkness without low-light annotations. The task, missing supervision, source domain, target domain, and desired output are all clear.

   - **Philosophy:** Great papers reduce a large field to a falsifiable question. A reader should know what success and failure mean before seeing the method.
   - **Apply to MANAR:** Frame MANAR around one primary question: *How much credible pre-flight evidence can an evidence-scoped software testbed provide for human-supervised aerial SAR before physical flight?* Routing, perception, authority boundaries, and failsafes should serve that question rather than appear as independent mini-papers.

2. **It motivates the problem with operational consequences, not generic importance language.** Darkness causes information loss and undermines machine vision in surveillance and driving; the paper shows a detector failing in Fig. 1.

   - **Philosophy:** Motivation becomes persuasive when the failure is observable and tied to a real decision.
   - **Apply to MANAR:** Open with the concrete conflict between rapid automated search and the danger of allowing unverified perception to command a rescue aircraft. Show a trace or diagram where a raw detection becomes an advisory candidate but cannot become a rescue declaration without human review.

3. **It identifies why obvious solutions fail.** Enhancement alone addresses pixel appearance, while generic domain adaptation attempts to cross an unusually large pixel-and-semantic gap in one step.

   - **Philosophy:** A method is compelling when it is the logical answer to a diagnosed failure mode, not just an assembly of fashionable modules.
   - **Apply to MANAR:** Explain why a fast detector alone is insufficient, why raw detection counts are not accuracy, why a shortest geometric route can conflict with tactical knowledge, and why a conceptual safety policy is not equivalent to verified flight behavior.

4. **Its central decomposition creates the paper's logic.** The high/low adaptation split appears in the motivation, architecture, losses, experiments, ablations, and conclusion.

   - **Philosophy:** A strong paper has a small number of concepts that recur consistently from question to evidence.
   - **Apply to MANAR:** Use one recurring evidence taxonomy: **implemented**, **measured**, and **planned**. Every table, claim, figure, and conclusion should make clear which category it belongs to.

5. **The method is explained at several levels.** The paper provides an intuitive diagram, mathematical constraints, concrete curve families, network architecture, losses, and implementation settings.

   - **Philosophy:** Readers need both conceptual compression and enough detail to reproduce the mechanism.
   - **Apply to MANAR:** Pair system diagrams with exact state transitions, route definitions, model input/output contracts, thresholds, benchmark scripts, seeds, and evidence artifacts. Avoid spending equations on standard formulas unless their implementation choice matters to the evaluation.

6. **The contribution claims map directly to the method and experiments.** The three introduction contributions correspond to low-level adaptation, high-level adaptation, and the complete system; later experiments test each.

   - **Philosophy:** A contribution is not a feature list. It is a claim whose evidence can be located.
   - **Apply to MANAR:** Limit contributions to claims that the repository can support: evidence-scoped architecture, fixed-seed route evaluation, and a diagnostic two-detector clip study. Treat the nine-step verification sequence and multisensor fusion as design specifications unless implemented and tested.

7. **It situates novelty against a broad and structured literature.** Related work is divided into low-light enhancement, face detection, dark object detection, unsupervised domain adaptation, and bidirectional adaptation.

   - **Philosophy:** Literature review should reveal the exact empty space the paper occupies.
   - **Apply to MANAR:** Organize prior work around aerial SAR systems, human-supervised autonomy, coverage/routing, aerial small-object detection, multispectral/thermal perception, and edge deployment. Explain how MANAR differs from each class. Twelve references are not enough for the current breadth.

8. **It uses the literature to justify design choices.** Prior enhancement aims at human visual quality; the proposed curve is designed for downstream machine vision.

   - **Philosophy:** Citations should participate in the argument, not decorate background paragraphs.
   - **Apply to MANAR:** Use aerial detection and edge-compute literature to justify input resolution, target size concerns, model selection, and the need for tracking. Use human-robot SAR literature to justify authority boundaries. Do not cite a model merely to name it.

9. **The mathematical development starts from explicit desired properties.** The enhancement family is constrained by endpoint preservation, monotonicity, coverage, and non-degenerate derivatives before candidate functions are compared.

   - **Philosophy:** Derive a design from requirements that can be inspected and tested.
   - **Apply to MANAR:** Define route objectives and safety invariants first. For example, route ordering should preserve operator order when optimization is rejected, lock after acceptance, and always include a return leg. Then connect each invariant to source and tests.

10. **The paper makes intermediate representations visible.** Figures show source, target, enhanced, degraded, and feature-adaptation states.

    - **Philosophy:** Visualizing intermediate states makes a causal story easier to audit.
    - **Apply to MANAR:** Show input frame, D-FINE output, YOLO output, and—once available—ground truth on the identical frame. Show route before greedy ordering, after greedy ordering, and after 2-opt refinement. Show state-machine transitions around RTL preemption.

11. **It evaluates a strong and diverse baseline set.** Twenty-four methods span detectors, enhancement, darkening, adaptation, and fully supervised learning.

    - **Philosophy:** A paper should test the strongest plausible alternative explanations and solution families.
    - **Apply to MANAR:** Two detectors are a pilot study, not an architecture verdict. Add at least one more convolutional detector, one small-object strategy such as slicing, one thermal-specific baseline if real LWIR data are used, and simple rule-based or tracking baselines. For routing, include manual order, greedy, greedy+2-opt, and an exact solution for small `n`.

12. **It includes an upper bound.** Fine-tuning with low-light annotations establishes the supervised ceiling and shows the unsupervised method is nearly competitive.

    - **Philosophy:** Readers need to know how far the proposed method remains from an ideal or more expensive solution.
    - **Apply to MANAR:** Once bounding boxes exist, include an in-domain fine-tuned detector as an upper bound. For routing at small `n`, compare with an exact solver. For software behavior, compare observed traces with formally specified expected traces.

13. **It uses an official metric and evaluation tool.** The paper reports mAP on official dataset splits with an identified evaluation tool.

    - **Philosophy:** Metrics must have a valid denominator, a defined matching rule, and a reproducible implementation.
    - **Apply to MANAR:** Raw detection events cannot be renamed recall, precision, F1, or false-positive rate. Until bounding-box ground truth exists, report only session latency, event counts, negative-clip detections, and positive-scenario coverage, with precise definitions.

14. **Its protocol is unusually concrete.** The datasets, official splits, batch size, optimizer, iteration count, learning-rate schedule, inference procedure, and metric are disclosed.

    - **Philosophy:** Reproducibility is a chain of small facts; omitting any one can make a result impossible to reproduce.
    - **Apply to MANAR:** State clip count, exact frames, resolution, frame rate, confidence threshold, NMS threshold, input size, model files/checksums, execution provider, what the latency timer includes, warm-up policy, run count, aggregation unit, and script path.

15. **Ablations test causal necessity.** The paper removes or swaps enhancement, jigsaw learning, contrastive learning, curve form, model capacity, and multiscale testing.

    - **Philosophy:** A final model score does not prove that any individual design choice mattered.
    - **Apply to MANAR:** Add threshold sweeps, input-resolution sweeps, NMS changes, per-condition breakdowns, detector-with/without tiling, route greedy vs. greedy+2-opt, and plan locking/override tests. Each claimed mechanism needs its own intervention.

16. **The ablations are interpreted, not merely tabulated.** The authors explain why jigsaw may outperform rotation and why specific curves are easier to train.

    - **Philosophy:** Numbers become knowledge only when connected to a plausible mechanism and alternative explanations.
    - **Apply to MANAR:** Avoid declaring that convolutional pyramids are inherently superior from two zero-shot models. Say that YOLO produced more events in this corpus, then list threshold calibration, export differences, model training, target scale, and postprocessing as competing explanations.

17. **The paper reports computational cost alongside task performance.** FLOPs, parameters, and time are compared for the enhancement module.

    - **Philosophy:** Engineering usefulness depends on the accuracy-cost frontier, not accuracy alone.
    - **Apply to MANAR:** Report model-session latency and file size, but also measure end-to-end preprocessing, inference, NMS, and rendering separately. Later add onboard power and thermal throttling on the actual flight computer.

18. **Qualitative evidence is comparative and controlled.** Many figures show the same input across methods, with consistent boxes and captions.

    - **Philosophy:** Qualitative examples are useful when the comparison holds the scene constant and the selection procedure is disclosed.
    - **Apply to MANAR:** Use exact frame-aligned panels for input, both detectors, and ground truth. Do not use a single attractive YOLO success image as the primary visual argument.

19. **The paper includes dedicated failure cases.** It shows false faces on heads, wheels, reflections, very small faces, and logos.

    - **Philosophy:** Failure disclosure defines the true operating envelope and makes the paper more credible.
    - **Apply to MANAR:** Create a failure taxonomy: no event on concealed targets, repeated detections of one person, modality mismatch, small targets, hot rocks, partial occlusion, camera motion, and threshold sensitivity. Show representative frames and quantify frequency after annotation.

20. **It compares the journal version with its earlier publication.** The authors state exactly what changed and quantify the gain.

    - **Philosophy:** Research maturity is demonstrated through explicit deltas, not by silently replacing an earlier story.
    - **Apply to MANAR:** Maintain an experiment manifest and revision table. When the benchmark changes, record which clips, models, thresholds, scripts, and metrics changed so results remain auditable.

21. **It tests generalization to several tasks and real-world images.** The method is applied to normal-light faces, generic detection, classification, and segmentation.

    - **Philosophy:** A general principle should survive outside the exact benchmark on which it was developed.
    - **Apply to MANAR:** Validate across a held-out location, different altitude, camera, time of day, terrain, and real physical RGB/LWIR sensors. Until then, call the current corpus diagnostic rather than general.

22. **It separates machine-vision quality from human visual quality.** An image can look better but perform worse for detection.

    - **Philosophy:** Optimize and measure the property the system actually needs.
    - **Apply to MANAR:** Separate visual attractiveness, raw event sensitivity, bounding-box accuracy, track persistence, geolocation error, operator decision time, and mission success. These are different outcomes.

23. **The conclusion is proportionate to the evidence.** It summarizes the mechanism and supported applications, then names remaining requirements for low-light images and future tasks.

    - **Philosophy:** A conclusion should not introduce stronger causal or operational claims than the experiments permit.
    - **Apply to MANAR:** Conclude that the artifact is a pre-flight evidence baseline, not a flight-ready rescue system. Repeat that no physical flight, end-to-end autonomy, sensor fusion, or annotated detector accuracy has been demonstrated.

24. **The paper is visually dense without becoming visually careless.** Tables have grouped categories, best results are emphasized, figures are legible, and captions say what to notice.

    - **Philosophy:** Page economy comes from information design, not smaller fonts or reduced margins.
    - **Apply to MANAR:** Remove low-value inventory tables, enlarge high-value comparison figures, shorten captions that merely repeat headings, keep table fonts readable, and let the IEEE class control margins. Never compress unsupported claims into tiny type.

25. **The title and abstract accurately advertise the central result.** The title is short and the abstract names the exact supervision setting, adaptation mechanism, and empirical conclusion.

    - **Philosophy:** The first 200 words are a contract with the reader.
    - **Apply to MANAR:** Shorten the title to the evidence-scoped systems thesis. In the abstract, define the problem, current artifact, fixed-seed route experiment, diagnostic vision result, and explicit limitations. Remove invented annotations, recall, F1, and repeated-run claims.

26. **The paper makes resources discoverable.** It provides a project site and evaluation-tool references.

    - **Philosophy:** A result is more valuable when others can inspect and rerun it.
    - **Apply to MANAR:** Link the public repository, commit an experiment script and machine-readable results, publish model hashes, document data licensing, and provide annotations or a lawful download path.

27. **The authors do not confuse a polished template with scientific quality.** The final TPAMI PDF is polished, but its strength comes from the evidence behind the layout.

    - **Philosophy:** Typography amplifies a rigorous argument; it cannot replace one.
    - **Apply to MANAR:** Journal-style abstract typography, running heads, balanced columns, and better captions are worthwhile. Copying TPAMI page numbers, copyright lines, acceptance history, or exact trim dimensions would be misleading and should not be done.

## Complete difference audit: why MANAR is currently weaker and how to close the gap

The wording "weaker" below refers to the evidentiary strength of the supplied manuscript, not the value of the MANAR project.

| Dimension | Reference paper | MANAR before revision | Why MANAR is weaker | Required improvement |
|---|---|---|---|---|
| Central thesis | One unsupervised low-light adaptation problem | Route optimization, software architecture, vision, telemetry, safety, and future sensors | The reader cannot identify the one claim that organizes the paper | Make evidence-scoped pre-flight validation the thesis; subordinate all subsystems to it |
| Contribution type | New vision method with evaluated modules | Systems prototype plus design proposals | Implemented and proposed features are mixed | Tag every claim as implemented, measured, or planned |
| Novelty argument | Explicit contrast with enhancement, darkening, and generic UDA | Mostly a feature inventory | The competitive research gap is unclear | State which prior SAR testbeds lack the exact evidence-scoping/authority/evaluation combination |
| Dataset count | Official WIDER FACE and DARK FACE splits | 26 local clips | Small local corpus limits external validity | Add held-out sites, sensors, conditions, and public provenance |
| Frame count | Official datasets and splits | PDF claims 4,218, while the local videos contain 6,240 frames | Basic corpus description is inconsistent | Derive the count from video metadata and report 26 x 240 = 6,240 |
| Resolution | Dataset/evaluation settings are consistent | PDF claims three different native resolutions, while all local clips are 1280 x 720 | The modality description is not supported by the files | Report the observed 1280 x 720 resolution; document any upstream sensor resolution separately only with evidence |
| Modality | Real low-light datasets with established provenance | Files are labeled RGB, Infra/Low-Light IR, and Thermal, but sensor provenance is not documented in the repository | Calling the corpus "multi-spectral" may imply physical calibrated sensors | Use "multi-condition" or "modality-labeled" until capture hardware and calibration are documented |
| Annotation | Official face boxes | Repository benchmark states that bounding-box normalization is future work | Current PDF invents annotated instances and denominators | Remove annotation, recall, precision, F1, and target-instance counts |
| Metric validity | Official mAP and PR curves | Raw frame-level detection-event totals | Raw events can repeat the same person and cannot measure recall | Define event count and positive-scenario coverage; annotate before reporting AP/recall |
| Negative evidence | Official labeled negatives within evaluation | Eleven clips named `No Target` or `False Positive`, with zero recorded events | Useful but not equivalent to a general false-positive rate | Report zero events on 11 negative clips/2,640 negative-designated frames, without extrapolating |
| Positive evidence | Ground-truth matched detections | At least one event in 12/15 vs. 6/15 positive-designated clips | Clip coverage is coarse and sensitive to one event | Call it positive-scenario coverage and supplement with annotations later |
| Timing | Method timings under disclosed hardware | Per-video mean `session.run` latency | The two graphs may include different operations; YOLO host-side NMS is outside the timer | Call it model-session latency and separately benchmark end-to-end stages |
| Repetition/statistics | Extensive trained experiments | PDF says ten repeated passes and gives SD values not present in the results log | Statistical claims are unsupported | Report mean and SD across the 26 recorded clip means, or rerun a scripted repeated benchmark |
| Detector comparison | Many baselines | YOLO11n vs. D-FINE-N | Two checkpoints cannot isolate convolution vs. transformer architecture | Add matched training, threshold calibration, more detectors, and per-condition confidence curves |
| Architecture conclusion | Causal modules tested by ablation | Attributes outcome to feature pyramids vs. query decoders | Model families differ in many confounders | Recast as an observation/hypothesis, not a causal conclusion |
| Route comparator | Not applicable | Greedy is compared to 2-opt but called a sub-optimality gap | 2-opt is a local heuristic, not ground-truth optimum | Say "relative excess over 2-opt-refined tour"; use exact optimum for small `n` |
| Route reproducibility | Not applicable | No route experiment script was checked in | Table cannot be independently regenerated | Add a fixed-seed script, geometry, start/home rule, trial count, and CSV |
| Runtime claim | Task performance and complexity are measured | `<50 microseconds` appears without a checked-in benchmark harness/hardware record | Runtime cannot be audited | Remove the number until a compiled benchmark logs CPU, compiler, flags, warmup, repetitions, and raw samples |
| Software verification | Not applicable | A 100% pass-rate matrix is present, but no automated test suite matching it is checked in | The table overstates available evidence | Replace it with an artifact/evidence table and add real automated tests before restoring pass rates |
| Memory claim | Not applicable | `0 memory leaks`/constant RSS is claimed without logs or tooling | Absolute memory-safety claims require evidence | Remove or attach sanitizer/Valgrind/heap-profiling logs and methodology |
| Candidate verification | Not applicable | Nine-step sequence is called deterministic contribution | Much of the sequence is conceptual rather than implemented in the source | Label it a target control policy; test each transition before claiming execution |
| Telemetry policy | Not applicable | Text says the policy is implemented, but core search finds no rate controller | Design and implementation are conflated | Label the model/policy analytical and planned; later add code and measured radio energy |
| Physical requirements | Not applicable | One-hour endurance and 2,000 m ceiling read like system capabilities | No airframe tests establish them | Label them design targets and separate AMSL/AGL/legal constraints |
| Related work | 80 citations across the full method neighborhood | 12 references | Broad paper claims rest on a narrow literature base | Add peer-reviewed SAR, aerial detection, thermal, small-object, edge, and verification literature |
| Ablations | Comprehensive | None | Design choices are not causally evaluated | Add threshold, resolution, tiling, route refinement, and state-machine ablations |
| Failure cases | Dedicated observed failure figure | Generic anticipated limitations | Readers cannot see where current models fail | Add aligned failure panels and a quantified taxonomy after annotation |
| Reproducibility | Project site, tool links, implementation detail | Some scripts/models/hashes exist, but results are partly manual and videos are ignored | Artifact lineage is incomplete | Add manifest, environment lockfile, commands, model provenance, raw CSV, and data license |
| Ethics/data governance | Dataset use is established | PDF asserts consent and release-on-request without a repository record | Unsupported ethics statements are serious | Retain such statements only with documentation; otherwise state provenance is to be documented before release |
| Scope of conclusion | Supported unsupervised method claims | Flight/system conclusions exceed pre-flight evidence | Operational readiness is implied | Conclude only source implementation and diagnostic software results |
| Title | Short problem statement | Long list of three contributions | Looks unfocused and consumes three lines | Use a shorter thesis-driven title |
| Abstract | Structured research story | Dense feature/metric inventory | Hard to distinguish fact, interpretation, and plan | Use problem-gap-artifact-protocol-results-limitations structure |
| Introduction | Problem and gap lead to method | Maturity table appears before a deep gap analysis | Administrative inventory interrupts the argument | Compress the scope table and expand the research gap |
| Section order | Related work -> method -> experiments -> conclusion | Requirements/architecture/mission/mathematics/route/perception/telemetry/safety/software/implementation | Reads like a design specification | Reorder around research questions and evidence, or clearly signal a systems-paper structure |
| Tables | Primarily comparisons/ablations | Many status, abbreviation, module, and policy inventories | Space is spent on catalogs rather than evidence | Move catalogs to appendix/repository; prioritize results and validity tables |
| Figures | Mechanism, comparisons, distributions, failures, applications | Diagrams and one success frame | Limited empirical visual evidence | Add controlled detector comparison and failure panels |
| Captions | Explain setup and visual interpretation | Mostly identify the object | Captions are less self-contained | Include dataset/condition, metric, direction of better performance, and scope caveat |
| Typography | Final TPAMI journal production | IEEE conference mode | Abstract is bold/dense and title is oversized/long | Use IEEE journal preprint mode, shorter title, author footnote, and balanced last page |
| Margins/page geometry | Publisher trim and furniture | Standard Letter conference template | Visually different, but manual imitation would be wrong | Let IEEEtran control geometry; do not use `geometry` to fake TPAMI trim |
| Running header | Journal/issue metadata | None | Preprint looks less journal-like | Add an honest `MANAR Technical Preprint` running head, not fake volume/issue data |
| First paragraph | Journal drop cap and compact section start | Standard paragraph | Minor visual difference | Use `\IEEEPARstart` in journal mode if it renders cleanly |
| Last page | References and biographies fill production pages | Half-empty final page | Poor column balance | Use `\balance` before references and remove low-value whitespace |
| Metadata | Title, subject, DOI, authors, dates | PDF metadata fields are blank | Searchability and archival quality are lower | Populate `pdftitle`, `pdfauthor`, `pdfsubject`, and `pdfkeywords` |
| Language precision | Mostly consistent technical terminology | `Probable Rescuee`, `Selected Closed Architecture`, and causal overstatements | Awkward terms reduce confidence | Use `probable-rescue classification`, `target architecture`, and calibrated causal language |

## Changes that can be applied now without inventing evidence

The following revisions are appropriate for the current repository and are implemented in the accompanying LaTeX revision:

- Shorten and refocus the title around an evidence-scoped pre-flight testbed.
- Switch from IEEE conference styling to an honest IEEE journal-style technical preprint, with a preprint running head rather than fake journal metadata.
- Rewrite the abstract and contribution list around repository-supported claims.
- Replace the vision corpus description with the observed 26 clips, 6,240 frames, 1280 x 720 resolution, and 24 fps.
- Replace unsupported recall, precision, F1, annotation, consent, and repeated-run claims with raw detection-event counts, positive-scenario coverage, negative-clip events, and per-clip model-session latency.
- Add a controlled input/D-FINE/YOLO qualitative comparison on the same frame, explicitly labeled illustrative rather than ground truth.
- Add a fixed-seed route benchmark script and replace "sub-optimality" with "relative excess over a 2-opt-refined tour."
- Remove unauditable microsecond runtime claims and 100% software pass-rate/memory claims.
- Convert the implementation section into an artifact and evidence-scope section.
- Add explicit threats to construct, internal, external, and reproducibility validity.
- Correct implementation/planning language for candidate verification, telemetry, sensors, and flight capabilities.
- Expand related work with the repository's curated peer-reviewed SAR, aerial detection, thermal, small-object, and edge-compute references.
- Populate PDF metadata, improve journal typography, and balance the final page.

## Improvements that cannot be honestly applied without new work

No amount of LaTeX editing can close the following gaps. They require experiments, data, or engineering artifacts:

1. **Bounding-box ground truth.** Annotate every evaluated frame or a documented sampled test set with person boxes, occlusion, target size, and modality labels.
2. **Valid detection metrics.** Compute AP, AP50, precision, recall, F1, false positives per image, and confidence intervals with a published matching protocol.
3. **Independent split.** Separate development, threshold-selection, and final test locations. Prevent adjacent frames from leaking across splits.
4. **Real sensor provenance.** Document camera model, spectral band, capture date/site, altitude, gimbal angle, and whether IR/thermal clips are true sensor captures or transformations.
5. **More baselines.** Evaluate at least three detector families and a small-object/tiling method under matched preprocessing and thresholds.
6. **End-to-end latency and power.** Time decode, resize, normalization, model session, postprocessing, rendering, and IPC on the intended onboard computer; log power and thermal throttling.
7. **Repeated measurements.** Use warmups, randomized run order, many repetitions, raw timing samples, and confidence intervals.
8. **Route optimality.** Use an exact solver for small instances and report optimality gaps; retain 2-opt as a practical refinement baseline.
9. **Automated software tests.** Check in unit/integration tests for route ordering, plan locking, RTL precedence, command clamps, malformed IPC, and battery state transitions.
10. **State-machine trace evidence.** Record expected and observed transition traces, including adversarial/fault-injection cases.
11. **Flight validation.** Progress through PX4 SITL, HITL, tethered tests, controlled outdoor flight, and operationally representative trials.
12. **Human-factors evaluation.** Measure operator workload, alert review time, missed candidates, false dispatches, and the benefit of authority gating.
13. **Failure-case dataset.** Curate and quantify concealment, clutter, motion blur, tiny targets, thermal crossover, hot rocks, and false-human shapes.
14. **Public artifact package.** Publish lawful data access, annotations, environment lockfile, exact commands, commit hash, raw results, and an experiment manifest.

## Target structure for a publication-level MANAR paper

A future submission should use a structure like this:

1. **Introduction** - operational problem, exact gap, research questions, contributions, evidence boundary.
2. **Related Work** - SAR platforms, supervised autonomy, coverage/routing, aerial perception, edge deployment.
3. **System and Safety Invariants** - implemented architecture, authority boundary, state invariants, implemented-vs-planned table.
4. **Methods** - route algorithm, detector pipeline, timing definition, candidate/track interface.
5. **Experimental Protocol** - data provenance, splits, annotations, models, hardware, metrics, statistics, reproducibility.
6. **Results** - route quality/optimality, detection accuracy, runtime/power, state-machine tests.
7. **Ablations and Sensitivity** - threshold, resolution, tiling, tracking, route refinement, policy choices.
8. **Failure Cases and Threats to Validity** - observed failures and limits.
9. **Discussion** - what is ready for SITL/HITL and what is not.
10. **Conclusion** - only claims supported by the evidence.

## Quality bar checklist

Before calling the MANAR paper comparable in research quality to the reference paper, every answer below should be "yes":

- Is there one central falsifiable question?
- Does each contribution correspond to a specific figure/table/test?
- Are implemented, measured, and planned claims visually distinguishable?
- Can every number be regenerated from a script and raw artifact?
- Do detection metrics use ground-truth matching and a valid denominator?
- Are training/development/test data separated by scenario or site, not adjacent frame?
- Are at least three serious baselines evaluated fairly?
- Is there an exact or supervised upper bound where applicable?
- Does each claimed mechanism have an ablation?
- Are failure cases shown and quantified?
- Are latency and power measured end to end on target hardware?
- Are software pass rates backed by checked-in tests and logs?
- Are data provenance, consent, and licensing documented?
- Are limitations stated in the abstract, results, and conclusion—not hidden at the end?
- Does the title describe the thesis rather than list subsystems?
- Are figures legible at one-column or two-column print size?
- Do captions explain the experimental setup and takeaway?
- Does the venue template control margins and typography without manual compression?
- Is the PDF metadata complete?
- Can an independent reader reproduce the central result from the repository?

The reference paper is a useful quality benchmark because nearly every major claim is connected to a controlled experiment, an ablation, a visual comparison, or an explicit limitation. MANAR can reach that level, but the path is primarily more disciplined evidence—not merely tighter margins or a more prestigious-looking template.
